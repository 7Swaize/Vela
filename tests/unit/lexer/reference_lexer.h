#ifndef VELA_REFERENCE_LEXER_H
#define VELA_REFERENCE_LEXER_H

// NOTE: THIS LEXER IS COMPLETELY AI GENERAED

// Behavior the reference defines (edit HERE when the language spec changes):
//   * Whitespace is " \t\r\n" and produces no token.
//   * Identifier / keyword: [A-Za-z_][A-Za-z0-9_]*, keyword if it matches a Kw* entry.
//   * Number: a maximal run of [A-Za-z0-9_] that starts with a digit.
//       - all digits                      -> IntLiteral
//       - digits '.' digits (all digits)  -> FloatLiteral ("1." is Int then Dot)
//       - anything else ("12ab", "1.5x")  -> Error spanning the whole word
//   * String: '"' ... '"'. A backslash inside a string escapes the next byte.
//       Newlines are allowed. Unterminated -> Error from the opening quote to EOF.
//   * Two-byte operators: == != <= >= << >> && ||
//       If an operator is immediately followed by another operator
//       (e.g. "===", "<<<", "&&&"), the first byte is an Error and lexing
//       resumes at the next byte.
//   * Single-byte tokens come from VELA_SINGLE_CHAR_TOKENS.
//   * Any other byte (':', '?', '\\' outside a string, NUL, bytes >= 0x80, ...)
//       is a one-byte Error token.
//   * Comments are not lexed yet (the grammar defines them, the lexer does not).
//   * The stream always ends with Eof at {n, n}.

#include <string_view>
#include <unordered_map>
#include <vector>

#include "frontend/lexer/token.h"

namespace vela::unittests::lex {
    using namespace vela::lex;

    inline bool is_alpha(const unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    inline bool is_digit(const unsigned char c) {
        return c >= '0' && c <= '9';
    }

    inline bool is_word(const unsigned char c) {
        return is_alpha(c) || is_digit(c);
    }

    inline bool is_space(const unsigned char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    inline TokenKind keyword_or_ident(const std::string_view word) {
        static const std::unordered_map<std::string_view, TokenKind> kKeywords = [] {
            std::unordered_map<std::string_view, TokenKind> m;
#define X(name, str)                                         \
    if (std::string_view(#name).starts_with("Kw")) {         \
        m.emplace(std::string_view(str), TokenKind::name);   \
    }
            VELA_TOKEN_KINDS(X)
#undef X
            return m;
        }();

        const auto it = kKeywords.find(word);
        return it == kKeywords.end() ? TokenKind::Identifier : it->second;
    }

    inline bool is_operator_at(const std::string_view s, const size_t i) {
        if (i + 1 >= s.size()) return false;
        const char a = s[i], b = s[i + 1];
        return ((a == '=' || a == '<' || a == '>' || a == '!') && b == '=')
               || (a == '&' && b == '&')
               || (a == '|' && b == '|')
               || (a == '<' && b == '<')
               || (a == '>' && b == '>');
    }

    inline TokenKind operator_kind(const char a, const char b) {
        if (a == '&') return TokenKind::AmpAmp;
        if (a == '|') return TokenKind::PipePipe;
        if (a == '=') return TokenKind::EqEq;
        if (a == '!') return TokenKind::ExclEq;
        if (a == '<') return b == '<' ? TokenKind::LtLt : TokenKind::LtEq;
        return b == '>' ? TokenKind::GtGt : TokenKind::GtEq; // a == '>'
    }

    inline TokenKind single_kind(const unsigned char c) {
        switch (c) {
#define X(ch, kind) \
    case ch:        \
        return TokenKind::kind;
            VELA_SINGLE_CHAR_TOKENS(X)
#undef X
            default:
                return TokenKind::Error;
        }
    }

    inline std::vector<Token> lex_from_reference(const std::string_view s) {
        std::vector<Token> out;
        const auto n = static_cast<LexPos>(s.size());
        LexPos i = 0;

        while (i < n) {
            const auto c = static_cast<unsigned char>(s[i]);

            if (is_space(c)) {
                ++i;
                continue;
            }

            if (c == '"') {
                LexPos j = i + 1;
                bool closed = false;
                while (j < n) {
                    if (s[j] == '\\') {
                        j += 2; // skip the escaped byte (may run past the end)
                    } else if (s[j] == '"') {
                        closed = true;
                        break;
                    } else {
                        ++j;
                    }
                }
                if (closed) {
                    out.push_back({i, j + 1, TokenKind::StringLiteral});
                    i = j + 1;
                } else {
                    out.push_back({i, n, TokenKind::Error});
                    i = n;
                }
                continue;
            }

            if (is_word(c)) {
                LexPos j = i;
                while (j < n && is_word(static_cast<unsigned char>(s[j]))) ++j;

                if (!is_digit(c)) {
                    out.push_back({i, j, keyword_or_ident(s.substr(i, j - i))});
                    i = j;
                    continue;
                }

                bool all_digits = true;
                for (LexPos k = i; k < j; ++k) {
                    all_digits &= is_digit(static_cast<unsigned char>(s[k]));
                }

                if (all_digits && j + 1 < n && s[j] == '.' && is_digit(static_cast<unsigned char>(s[j + 1]))) {
                    LexPos k = j + 1;
                    while (k < n && is_word(static_cast<unsigned char>(s[k]))) ++k;
                    bool frac_digits = true;
                    for (LexPos m = j + 1; m < k; ++m) {
                        frac_digits &= is_digit(static_cast<unsigned char>(s[m]));
                    }
                    out.push_back({i, k, frac_digits ? TokenKind::FloatLiteral : TokenKind::Error});
                    i = k;
                } else {
                    out.push_back({i, j, all_digits ? TokenKind::IntLiteral : TokenKind::Error});
                    i = j;
                }
                continue;
            }

            if (is_operator_at(s, i)) {
                if (is_operator_at(s, i + 1)) {
                    out.push_back({i, i + 1, TokenKind::Error});
                    i += 1;
                } else {
                    out.push_back({i, i + 2, operator_kind(s[i], s[i + 1])});
                    i += 2;
                }
                continue;
            }

            out.push_back({i, i + 1, single_kind(c)});
            ++i;
        }

        out.push_back({n, n, TokenKind::Eof});
        return out;
    }
} // namspace vela::unittests:lex

#endif // VELA_REFERENCE_LEXER_H
