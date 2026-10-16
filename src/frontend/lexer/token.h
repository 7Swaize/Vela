#ifndef VELA_TOKEN_H
#define VELA_TOKEN_H

#include <cstdint>
#include <string_view>

namespace vela::lex {
#define VELA_TOKEN_KINDS(X)       \
    X(Eof, "<eof>")               \
    X(Error, "<error>")           \
    X(Identifier, "<identifier>") \
    X(IntLiteral, "<int>")        \
    X(FloatLiteral, "<float>")    \
    X(StringLiteral, "<string>")  \
    X(KwStruct, "struct")         \
    X(KwClass, "class")           \
    X(KwFn, "fn")                 \
    X(KwIf, "if")                 \
    X(KwElse, "else")             \
    X(KwFor, "for")               \
    X(KwWhile, "while")           \
    X(KwReturn, "return")         \
    X(KwTrue, "true")             \
    X(KwFalse, "false")           \
    X(KwNull, "null")             \
    X(KwThis, "this")             \
    X(KwNew, "new")               \
    X(KwVoid, "void")             \
    X(KwInt, "int")               \
    X(KwFloat, "float")           \
    X(KwBool, "bool")             \
    X(KwString, "string")         \
    X(LParen, "(")                \
    X(RParen, ")")                \
    X(LBrace, "{")                \
    X(RBrace, "}")                \
    X(LBracket, "[")              \
    X(RBracket, "]")              \
    X(Comma, ",")                 \
    X(Dot, ".")                   \
    X(Semicolon, ";")             \
    X(Plus, "+")                  \
    X(Minus, "-")                 \
    X(Star, "*")                  \
    X(Slash, "/")                 \
    X(Percent, "%")               \
    X(Excl, "!")                  \
    X(Tilde, "~")                 \
    X(Amp, "&")                   \
    X(Pipe, "|")                  \
    X(Caret, "^")                 \
    X(Lt, "<")                    \
    X(Gt, ">")                    \
    X(Assign, "=")                \
    X(AmpAmp, "&&")               \
    X(PipePipe, "||")             \
    X(EqEq, "==")                 \
    X(ExclEq, "!=")               \
    X(LtEq, "<=")                 \
    X(GtEq, ">=")                 \
    X(LtLt, "<<")                 \
    X(GtGt, ">>")

#define VELA_SINGLE_CHAR_TOKENS(X) \
    X('(', LParen)                 \
    X(')', RParen)                 \
    X('{', LBrace)                 \
    X('}', RBrace)                 \
    X('[', LBracket)               \
    X(']', RBracket)               \
    X(',', Comma)                  \
    X('.', Dot)                    \
    X(';', Semicolon)              \
    X('+', Plus)                   \
    X('-', Minus)                  \
    X('*', Star)                   \
    X('/', Slash)                  \
    X('%', Percent)                \
    X('!', Excl)                   \
    X('~', Tilde)                  \
    X('&', Amp)                    \
    X('|', Pipe)                   \
    X('^', Caret)                  \
    X('<', Lt)                     \
    X('>', Gt)                     \
    X('=', Assign)

#define VELA_LEX_ERROR_KINDS(X)                  \
    X(InvalidCharacter, "invalid character")     \
    X(MalformedNumber, "malformed number")       \
    X(UnterminatedString, "unterminated string") \
    X(UnterminatedBlockComment, "unterminated block comment")

    enum class TokenKind : uint8_t {
#define X(name, str) name,
        VELA_TOKEN_KINDS(X)
#undef X
    };

    constexpr std::string_view to_string(const TokenKind k) {
        switch (k) {
#define X(name, str)      \
    case TokenKind::name: \
        return str;
            VELA_TOKEN_KINDS(X)
#undef X
        }

        return "<?>";
    }

    using LexPos = uint32_t;

    // Currently we track absolute positions because its easier (and faster in the single compilation sense).
    // If this were to get transfered to an IDE, we should track offsets to invalidation of every token on edit.
    struct Token {
        LexPos start, end;
        TokenKind kind;

        constexpr bool operator==(const Token &) const = default;
    };

    enum class LexErrorKind : uint8_t {
#define X(name, str) name,
        VELA_LEX_ERROR_KINDS(X)
#undef X
    };

    constexpr std::string_view to_string(const LexErrorKind k) {
        switch (k) {
#define X(name, str)         \
    case LexErrorKind::name: \
        return str;
            VELA_LEX_ERROR_KINDS(X)
#undef X
        }

        return "?";
    }

    struct LexDiag {
        uint32_t offset;
        uint32_t length;
        LexErrorKind kind;

        constexpr bool operator==(const LexDiag &) const = default;
    };


    constexpr inline int kLongestKeyword = [] {
        int max_len = 0;

#define X(name, str)                                                    \
    if constexpr (std::string_view(#name).starts_with("Kw")) {          \
        const int len = static_cast<int>(std::string_view(str).size()); \
        if (len > max_len) {                                            \
            max_len = len;                                              \
        }                                                               \
    }
        VELA_TOKEN_KINDS(X)
#undef X

        return max_len;
    }();

    constexpr inline int kShortestKeyword = [] {
        int min_len = 0;

#define X(name, str)                                                    \
    if constexpr (std::string_view(#name).starts_with("Kw")) {          \
        const int len = static_cast<int>(std::string_view(str).size()); \
        if (len < min_len) {                                            \
            min_len = len;                                              \
        }                                                               \
    }
        VELA_TOKEN_KINDS(X)
#undef X

        return min_len;
    }();

} // namespace vela::lex

#endif // VELA_TOKEN_H
