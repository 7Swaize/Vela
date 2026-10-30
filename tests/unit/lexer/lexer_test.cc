// ReSharper disable CppUseDesignatedInitializers
#include <gtest/gtest.h>

#include <cstddef>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "frontend/lexer/lex.h"
#include "frontend/source_buffer.h"
#include "reference_lexer.h"
#include "shared/helpers.h"

namespace {
    using namespace vela::lex;
    using namespace std::string_view_literals;

    using TokenVec = std::vector<Token>;
    using KindsVec = std::vector<TokenKind>;
    using K = TokenKind;

    constexpr vela::shared::SourceBufferCreationConfig kBufferCreationConfig{
        .paddingBeyondBoundary = 2,
        .additionalPaddingValue = '\0',
        .maxFileSize = std::numeric_limits<backend::MaskType>::max(),
    };

    constexpr std::array<std::string_view, 28> kDifferentialTestCases = {
        "x = 1.5; y = 12ab; z = 1.5x; w = 1.2.3; v = 1.;",
        "",
        " ",
        "x",
        "fn main() { return 0; }",
        "struct Point { int x; int y; }",
        "class Player { fn void move(int dx, int dy) { x = x + dx; y = y + dy; } }",
        "fn int main() { Point p = new Point { x = 1, y = 2.5 }; return p.x[1]; }",
        "if (a >= 1 && !done || b << 2 == c) { return null; } else { this.x = 3; }",
        "for (int i = 0; i < 10; i = i + 1) { while (true) { } }",
        "string s = \"hello \\\"world\\\"\"; string t = \"\";", // NOLINT(*-raw-string-literal)
        "\"if == 1.5 ; // not tokens\"",
        "\"a\\\\\" x \"b\"", // NOLINT(*-raw-string-literal)
        "\"unterminated",
        "\"unterminated escape \\\"", // NOLINT(*-raw-string-literal)
        "\"trailing backslash\\",
        "x = 1.5; y = 12ab; z = 1.5x; w = 1.2.3; v = 1.;",
        "a===b; c<<<d; e&&&f; g<<=h; i<=>j; k!==l",
        "== != <= >= << >> && || = < > ! & |",
        "(){}[],.;+-*/%!~&|^<>=",
        "a:b?c@d#e$f`g'h\\i",
        "\xc3\xa9\xff\x80 caf\xc3\xa9",
        "nul\0byte"sv,
        "tab\tcr\rlf\nmix  \t \r\n end",
        "while_ if_ _if if1 iff Fn fn_ _fn0",
        "0 00 007 123456789 1.0 0.0 10.01 1_000",
        "a.b.c.d[e](f)",
        "\"multi\nline\nstring\" tail"
    };

    TokenVec inv_real(const std::string_view src) {
        return lex(vela::shared::FileSourceBuffer<backend::SimdWord>::from_string(src, kBufferCreationConfig));
    }

    TokenVec inv_ref(const std::string_view src) {
        return vela::unittests::lex::lex_from_reference(src);
    }

    KindsVec kinds(const std::string_view src) {
        std::vector<TokenKind> k;
        for (const Token &t: inv_ref(src)) {
            k.push_back(t.kind);
        }

        return k;
    }

    // Returns an empty string when the stream is well formed.
    std::string malformed(const std::string_view src, const TokenVec &tokens) {
        const auto n = static_cast<LexPos>(src.size());

        if (tokens.empty()) {
            return "no tokens (expected at least an EOF token)";
        }

        const Token &last = tokens.back();
        if (last.kind != TokenKind::Eof || last.start != n || last.end != n) {
            return "stream must end with EOF at {n, n}";
        }

        LexPos prev_end = 0;
        for (std::size_t i = 0; i + 1 < tokens.size(); ++i) {
            const auto &[start, end, kind] = tokens[i];
            const std::string id = "#" + std::to_string(i);

            if (kind == TokenKind::Eof) {
                return "EOF before end of stream (token " + id + ")";
            }
            if (start >= end) {
                return "empty or inverted token " + id;
            }
            if (start < prev_end) {
                return "overlapping tokens at " + id;
            }
            if (end > n) {
                return "token " + id + " extends past end of input";
            }

            prev_end = end;
        }

        return {};
    }

    testing::AssertionResult same_as_reference(const std::string_view src) {
        const TokenVec got = inv_real(src);
        const TokenVec want = vela::unittests::lex::lex_from_reference(src);

        if (const std::string problem = malformed(src, got); !problem.empty()) {
            return testing::AssertionFailure()
                    << "malformed token stream: " << problem
                    << "\ninput: \"" << src << "\""
                    << "\ngot:  " << vela::shared::collection_to_string(got)
                    << "\nwant: " << vela::shared::collection_to_string(want);
        }

        if (got != want) {
            return testing::AssertionFailure()
                    << "tokens differ from reference"
                    << "\ninput: \"" << src << "\""
                    << "\ngot:  " << vela::shared::collection_to_string(got)
                    << "\nwant: " << vela::shared::collection_to_string(want);
        }

        return testing::AssertionSuccess();
    }

    bool fails(const std::string_view src) {
        return !same_as_reference(src);
    }
}


TEST(ReferenceLexer, EmptyAndWhitespaceOnly) {
    EXPECT_EQ(inv_ref(""), (TokenVec{{0, 0, K::Eof}}));
    EXPECT_EQ(inv_ref(" \t\r\n"), (TokenVec{{4, 4, K::Eof}}));
}

TEST(ReferenceLexer, KeywordsAndIdentifiers) {
#define X(name, str)                                                                \
    if (std::string_view(#name).starts_with("Kw")) {                                \
        EXPECT_EQ(kinds(str), (KindsVec{K::name, K::Eof})) << str;                  \
        EXPECT_EQ(kinds(std::string(str) + "x"), (KindsVec{K::Identifier, K::Eof}));\
        EXPECT_EQ(kinds("_" str), (KindsVec{K::Identifier, K::Eof}));               \
    }
    VELA_TOKEN_KINDS(X)
#undef X
    EXPECT_EQ(kinds("Fn If"), (KindsVec{K::Identifier, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("if1 _ __x a_b9"), (KindsVec{K::Identifier, K::Identifier, K::Identifier, K::Identifier, K::Eof}));
    EXPECT_EQ(inv_ref("fn main"), (TokenVec{{0, 2, K::KwFn}, {3, 7, K::Identifier}, {7, 7, K::Eof}}));
}

TEST(ReferenceLexer, Numbers) {
    EXPECT_EQ(inv_ref("0 42 1.5"), (TokenVec{{0, 1, K::IntLiteral}, {2, 4, K::IntLiteral}, {5, 8, K::FloatLiteral}, {8, 8, K::Eof}}));
    EXPECT_EQ(kinds("007"), (KindsVec{K::IntLiteral, K::Eof}));
    EXPECT_EQ(kinds("1."), (KindsVec{K::IntLiteral, K::Dot, K::Eof}));
    EXPECT_EQ(kinds("1.x"), (KindsVec{K::IntLiteral, K::Dot, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds(".5"), (KindsVec{K::Dot, K::IntLiteral, K::Eof}));
    EXPECT_EQ(kinds("1.2.3"), (KindsVec{K::FloatLiteral, K::Dot, K::IntLiteral, K::Eof}));
    EXPECT_EQ(inv_ref("12ab"), (TokenVec{{0, 4, K::Error}, {4, 4, K::Eof}}));
    EXPECT_EQ(inv_ref("1.5x"), (TokenVec{{0, 4, K::Error}, {4, 4, K::Eof}}));
    EXPECT_EQ(kinds("p.x"), (KindsVec{K::Identifier, K::Dot, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, Strings) {
    EXPECT_EQ(inv_ref(R"("abc")"), (TokenVec{{0, 5, K::StringLiteral}, {5, 5, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("")"), (TokenVec{{0, 2, K::StringLiteral}, {2, 2, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("a\"b")"), (TokenVec{{0, 6, K::StringLiteral}, {6, 6, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("a\\")"), (TokenVec{{0, 5, K::StringLiteral}, {5, 5, K::Eof}})); // \\ then closing quote
    EXPECT_EQ(inv_ref("\"a\nb\""), (TokenVec{{0, 5, K::StringLiteral}, {5, 5, K::Eof}}));
    EXPECT_EQ(kinds(R"("a" "b")"), (KindsVec{K::StringLiteral, K::StringLiteral, K::Eof}));
    EXPECT_EQ(kinds(R"(x"a"y)"), (KindsVec{K::Identifier, K::StringLiteral, K::Identifier, K::Eof}));
    // Operators and keywords inside a string are just string content.
    EXPECT_EQ(kinds(R"("if == 1.5 ;")"), (KindsVec{K::StringLiteral, K::Eof}));
}

TEST(ReferenceLexer, UnterminatedStrings){
    EXPECT_EQ(inv_ref(R"("a\\")"), (TokenVec{{0, 5, K::StringLiteral}, {5, 5, K::Eof}})); // \\ then closing quote
    EXPECT_EQ(inv_ref(R"("abc)"), (TokenVec{{0, 4, K::Error}, {4, 4, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("abc\")"), (TokenVec{{0, 6, K::Error}, {6, 6, K::Eof}}));
    EXPECT_EQ(inv_ref("\"a\\"), (TokenVec{{0, 3, K::Error}, {3, 3, K::Eof}})); // backslash is the last byte
    EXPECT_EQ(inv_ref("\""), (TokenVec{{0, 1, K::Error}, {1, 1, K::Eof}}));
}

TEST(ReferenceLexer, Operators) {
    EXPECT_EQ(kinds("== != <= >= << >> && ||"), (KindsVec{K::EqEq, K::ExclEq, K::LtEq, K::GtEq, K::LtLt, K::GtGt, K::AmpAmp, K::PipePipe, K::Eof}));
    EXPECT_EQ(kinds("a<b"), (KindsVec{K::Identifier, K::Lt, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("x=y"), (KindsVec{K::Identifier, K::Assign, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("= ="), (KindsVec{K::Assign, K::Assign, K::Eof}));
    EXPECT_EQ(kinds("& &"), (KindsVec{K::Amp, K::Amp, K::Eof}));
    EXPECT_EQ(kinds("<=>"), (KindsVec{K::LtEq, K::Gt, K::Eof}));
    EXPECT_EQ(kinds("!x"), (KindsVec{K::Excl, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("a==!b"), (KindsVec{K::Identifier, K::EqEq, K::Excl, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, MalformedOperatorChains) {
    // An operator directly followed by another operator is malformed: all bytes defining the operator are errored, and lexing resumes after block
    EXPECT_EQ(inv_ref("==="), (TokenVec{{0, 3, K::Error}, {3, 3, K::Eof}}));
    EXPECT_EQ(inv_ref("<<<"), (TokenVec{{0, 3, K::Error}, {3, 3, K::Eof}}));
    EXPECT_EQ(inv_ref("<<="), (TokenVec{{0, 3, K::Error}, {3, 3, K::Eof}}));
    EXPECT_EQ(inv_ref("&&&"), (TokenVec{{0, 3, K::Error}, {3, 3, K::Eof}}));
}

TEST(ReferenceLexer, SingleCharTokens) {
#define X(ch, kind) EXPECT_EQ(inv_ref(std::string(1, ch)), (TokenVec{{0, 1, K::kind}, {1, 1, K::Eof}})) << ch;
    VELA_SINGLE_CHAR_TOKENS(X)
#undef X
}

TEST(ReferenceLexer, InvalidBytes) {
    for (const char c : {':', '?', '@', '#', '$', '`', '\'', '\\'}) {
        EXPECT_EQ(inv_ref(std::string(1, c)), (TokenVec{{0, 1, K::Error}, {1, 1, K::Eof}})) << c;
    }
    EXPECT_EQ(inv_ref(std::string_view("\0", 1)), (TokenVec{{0, 1, K::Error}, {1, 1, K::Eof}}));
    EXPECT_EQ(inv_ref("\xff"), (TokenVec{{0, 1, K::Error}, {1, 1, K::Eof}}));
    // Each invalid byte is its own token (a UTF-8 sequence is several Errors).
    EXPECT_EQ(kinds("\xc3\xa9"), (KindsVec{K::Error, K::Error, K::Eof}));
    EXPECT_EQ(kinds("a:b"), (KindsVec{K::Identifier, K::Error, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, Whitespace) {
    EXPECT_EQ(inv_ref("a\tb\r\nc"), (TokenVec{{0, 1, K::Identifier}, {2, 3, K::Identifier}, {5, 6, K::Identifier}, {6, 6, K::Eof}}));
}

TEST(Differential, Base) {
    for (const std::string_view view : kDifferentialTestCases) {
        EXPECT_TRUE(same_as_reference(view));
    }
}

TEST(Differential, SlidAcrossBlockBoundaries) {
    for (const std::string_view view : kDifferentialTestCases) {
        constexpr std::size_t kMaxShift = sizeof(backend::MaskType);
        for (std::size_t i = 0; i < kMaxShift; ++i) {
            const std::string shifted = std::string(i, ' ').append(view);
            ASSERT_TRUE(same_as_reference(shifted));
        }
    }
}

TEST(Differential, EverySingleByte) {
    for (int i = 0; i < 256; ++i) {
        ASSERT_TRUE(same_as_reference(std::string_view(reinterpret_cast<const char *>(&i), 1)));
    }
}