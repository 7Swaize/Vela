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

    constexpr std::string_view kDifferentialTestCases[] = {
        // Base
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
        "\"multi\nline\nstring\" tail",
        // Whitespace trivia
        "   ",
        "\t\t\t",
        "\r\r\n\n",
        "a  \t\t \n\n\r\r b",
        // Line comments
        "//",
        "// hi",
        "// hi\nx",
        "x // c\ny",
        "// c\r\nx",
        "///\n//\n",
        "// \"not a string\nx",
        "// \xff\x80:?@#\\ \n",
        "// trailing backslash\\",
        // Block comments
        "/* c */",
        "/**/",
        "/***/",
        "/*/",
        "/*/ x */",
        "/* * / */",
        "/* a\nb */ x",
        "x/*c*/y",
        "a/**/b",
        "/* /* */ */",
        "/* \" */ x \"s\"",
        "/* unterminated",
        "/*",
        "/",
        "a / b /c/ d",
        "\"/* not a comment */\" \"// nor this\"",
        "\"a\"//c\n\"b\"/*d*/",
        "a<=/*c*/b ==// c\n!=c",
        "int x = 1; // trailing\nint y = 2; /* block\nspanning */ int z;",
    };

    std::string build_line_comment_cross_block(const std::size_t body) {
        return "//" + std::string(body, 'x') + "\nid";
    }

    std::string block_comment_cross_block(const std::size_t body) {
        return "/*" + std::string(body, 'x') + "*/id";
    }

    std::string block_comment_with_quote_cross_block(const std::size_t body) {
        return "/*" + std::string(body, 'x') + "\"" + std::string(body, 'y') + "*/ \"s\"";
    }

    std::string string_with_comment_markers_cross_block(const std::size_t body) {
        return "\"" + std::string(body, 'x') + "//" + std::string(body, 'y') + "/*\" id";
    }

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

    KindsVec filter_non_trivia(const std::string_view src) {
        std::vector<TokenKind> k;
        for (const Token &token: inv_ref(src)) {
            if (!is_trivia(token.kind)) {
                k.push_back(token.kind);
            }
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
        if (last.kind != TokenKind::Eof || last.width != 0) {
            return "stream must end width zero-width EOF";
        }

        LexPos covered = 0;
        for (std::size_t i = 0; i + 1 < tokens.size(); ++i) {
            const auto &[width, kind] = tokens[i];
            const std::string id = "#" + std::to_string(i);

            if (kind == TokenKind::Eof) {
                return "EOF before end of stream (token " + id + ")";
            }
            if (width == 0) {
                return "zero-width token " + id;
            }
            if (width > n) {
                return "token " + id + " is wider than the whole input (width underflow?)";
            }

            covered += width;
            if (covered > n) {
                return "token" + id + " extends past end of input";
            }
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
}


TEST(ReferenceLexer, EmptyAndWhitespaceOnly) {
    EXPECT_EQ(inv_ref(""), (TokenVec{{0, K::Eof}}));
    EXPECT_EQ(inv_ref(" \t\r\n"), (TokenVec{{1, K::SpaceTrivia}, {1, K::TabTrivia}, {1, K::CarriageReturnTrivia}, {1, K::NewlineTrivia}, {0, K::Eof}}));
}

TEST(ReferenceLexer, KeywordsAndIdentifiers) {
#define X(name, str)                                                                    \
    if (std::string_view(#name).starts_with("Kw")) {                                    \
        EXPECT_EQ(kinds(str), (KindsVec{K::name, K::Eof})) << str;                      \
        EXPECT_EQ(kinds(std::string(str) + "x"), (KindsVec{K::Identifier, K::Eof}));    \
        EXPECT_EQ(kinds("_" str), (KindsVec{K::Identifier, K::Eof}));                   \
    }
    VELA_CORE_TOKEN_KINDS(X)
#undef X
    EXPECT_EQ(filter_non_trivia("Fn If"), (KindsVec{K::Identifier, K::Identifier, K::Eof}));
    EXPECT_EQ(filter_non_trivia("if1 _ __x a_b9"), (KindsVec{K::Identifier, K::Identifier, K::Identifier, K::Identifier, K::Eof}));
    EXPECT_EQ(inv_ref("fn main"), (TokenVec{{2, K::KwFn}, {1, K::SpaceTrivia}, {4, K::Identifier}, {0, K::Eof}}));
}

TEST(ReferenceLexer, TriviaPredicate) {
    EXPECT_TRUE(vela::lex::is_trivia(K::NewlineTrivia));
    EXPECT_TRUE(vela::lex::is_trivia(K::SpaceTrivia));
    EXPECT_TRUE(vela::lex::is_trivia(K::TabTrivia));
    EXPECT_TRUE(vela::lex::is_trivia(K::CarriageReturnTrivia));
    EXPECT_TRUE(vela::lex::is_trivia(K::CommentTrivia));
    EXPECT_TRUE(vela::lex::is_trivia(K::MultilineCommentTrivia));
    EXPECT_FALSE(vela::lex::is_trivia(K::Identifier));
    EXPECT_FALSE(vela::lex::is_trivia(K::Slash));
    EXPECT_FALSE(vela::lex::is_trivia(K::Error));
    EXPECT_FALSE(vela::lex::is_trivia(K::Eof));
}

TEST(ReferenceLexer, Numbers) {
    EXPECT_EQ(inv_ref("0 42 1.5"), (TokenVec{{1, K::IntLiteral}, {1, K::SpaceTrivia}, {2, K::IntLiteral}, {1, K::SpaceTrivia}, {3, K::FloatLiteral}, {0, K::Eof}}));
    EXPECT_EQ(kinds("007"), (KindsVec{K::IntLiteral, K::Eof}));
    EXPECT_EQ(kinds("1."), (KindsVec{K::IntLiteral, K::Dot, K::Eof}));
    EXPECT_EQ(kinds("1.x"), (KindsVec{K::IntLiteral, K::Dot, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds(".5"), (KindsVec{K::Dot, K::IntLiteral, K::Eof}));
    EXPECT_EQ(kinds("1.2.3"), (KindsVec{K::FloatLiteral, K::Dot, K::IntLiteral, K::Eof}));
    EXPECT_EQ(inv_ref("12ab"), (TokenVec{{4, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("1.5x"), (TokenVec{{4, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(kinds("p.x"), (KindsVec{K::Identifier, K::Dot, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, Strings) {
    EXPECT_EQ(inv_ref(R"("abc")"), (TokenVec{{5, K::StringLiteral}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("")"), (TokenVec{{2, K::StringLiteral}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("a\"b")"), (TokenVec{{6, K::StringLiteral}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("a\\")"), (TokenVec{{5, K::StringLiteral}, {0, K::Eof}})); // \\ then closing quote
    EXPECT_EQ(inv_ref("\"a\nb\""), (TokenVec{{5, K::StringLiteral}, {0, K::Eof}}));
    EXPECT_EQ(kinds(R"("a" "b")"), (KindsVec{K::StringLiteral, K::SpaceTrivia, K::StringLiteral, K::Eof}));
    EXPECT_EQ(kinds(R"(x"a"y)"), (KindsVec{K::Identifier, K::StringLiteral, K::Identifier, K::Eof}));

    // Operators and keywords inside a string are just string content.
    EXPECT_EQ(kinds(R"("if == 1.5 ;")"), (KindsVec{K::StringLiteral, K::Eof}));
    // So are whitespace and comment markers.
    EXPECT_EQ(kinds("\"a \t\n b\""), (KindsVec{K::StringLiteral, K::Eof}));
    EXPECT_EQ(kinds(R"("// no")"), (KindsVec{K::StringLiteral, K::Eof}));
    EXPECT_EQ(kinds(R"("/* no */")"), (KindsVec{K::StringLiteral, K::Eof}));
}

TEST(ReferenceLexer, UnterminatedStrings){
    EXPECT_EQ(inv_ref(R"("a\\")"), (TokenVec{{5, K::StringLiteral}, {0, K::Eof}})); // \\ then closing quote
    EXPECT_EQ(inv_ref(R"("abc)"), (TokenVec{{4, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref(R"("abc\")"), (TokenVec{{6, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("\"a\\"), (TokenVec{{3, K::Error}, {0, K::Eof}})); // backslash is the last byte
    EXPECT_EQ(inv_ref("\""), (TokenVec{{1, K::Error}, {0, K::Eof}}));

    // Unterminated string swallows everything after it.
    EXPECT_EQ(inv_ref("\"a // b\n/* c"), (TokenVec{{static_cast<vela::lex::LexPos>(sizeof("\"a // b\n/* c") - 1), K::Error}, {0, K::Eof}}));
}

TEST(ReferenceLexer, Operators) {
    EXPECT_EQ(filter_non_trivia("== != <= >= << >> && ||"), (KindsVec{K::EqEq, K::ExclEq, K::LtEq, K::GtEq, K::LtLt, K::GtGt, K::AmpAmp, K::PipePipe, K::Eof}));
    EXPECT_EQ(kinds("a<b"), (KindsVec{K::Identifier, K::Lt, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("x=y"), (KindsVec{K::Identifier, K::Assign, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("= ="), (KindsVec{K::Assign, K::SpaceTrivia, K::Assign, K::Eof}));
    EXPECT_EQ(kinds("& &"), (KindsVec{K::Amp, K::SpaceTrivia, K::Amp, K::Eof}));
    EXPECT_EQ(kinds("<=>"), (KindsVec{K::LtEq, K::Gt, K::Eof}));
    EXPECT_EQ(kinds("!x"), (KindsVec{K::Excl, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("a==!b"), (KindsVec{K::Identifier, K::EqEq, K::Excl, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, MalformedOperatorChains) {
    // An operator directly followed by another operator is malformed: all bytes defining the operator are errored, and lexing resumes after block
    EXPECT_EQ(inv_ref("==="), (TokenVec{{3, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("<<<"), (TokenVec{{3, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("<<="), (TokenVec{{3, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("&&&"), (TokenVec{{3, K::Error}, {0, K::Eof}}));
}

TEST(ReferenceLexer, SingleCharTokens) {
#define X(ch, kind) EXPECT_EQ(inv_ref(std::string(1, ch)), (TokenVec{{1, K::kind}, {0, K::Eof}})) << ch;
    VELA_SINGLE_CHAR_TOKENS(X)
#undef X
}

TEST(ReferenceLexer, InvalidBytes) {
    for (const char c : {':', '?', '@', '#', '$', '`', '\'', '\\'}) {
        EXPECT_EQ(inv_ref(std::string(1, c)), (TokenVec{{1, K::Error}, {0, K::Eof}})) << c;
    }
    EXPECT_EQ(inv_ref(std::string_view("\0", 1)), (TokenVec{{1, K::Error}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("\xff"), (TokenVec{{1, K::Error}, {0, K::Eof}}));
    // Each invalid byte is its own token (a UTF-8 sequence is several Errors).
    EXPECT_EQ(kinds("\xc3\xa9"), (KindsVec{K::Error, K::Error, K::Eof}));
    EXPECT_EQ(kinds("a:b"), (KindsVec{K::Identifier, K::Error, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, WhitespaceTrivia) {
    EXPECT_EQ(inv_ref("a\tb\r\nc"), (TokenVec{{1, K::Identifier}, {1, K::TabTrivia}, {1, K::Identifier}, {1, K::CarriageReturnTrivia}, {1, K::NewlineTrivia}, {1, K::Identifier}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("a b"), (TokenVec{{1, K::Identifier}, {1, K::SpaceTrivia}, {1, K::Identifier}, {0, K::Eof}}));
}

TEST(ReferenceLexer, WhitespaceRunsCollapseByKind) {
    EXPECT_EQ(inv_ref("   "), (TokenVec{{3, K::SpaceTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("\t\t"), (TokenVec{{2, K::TabTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("\n\n\n"), (TokenVec{{3, K::NewlineTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("\r\r"), (TokenVec{{2, K::CarriageReturnTrivia}, {0, K::Eof}}));
}

TEST(ReferenceLexer, ChangeOfWhitespaceInvalidatesRun) {
    EXPECT_EQ(inv_ref("  \t  "), (TokenVec{{2, K::SpaceTrivia}, {1, K::TabTrivia}, {2, K::SpaceTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("\r\n\r\n"), (TokenVec{{1, K::CarriageReturnTrivia}, {1, K::NewlineTrivia}, {1, K::CarriageReturnTrivia}, {1, K::NewlineTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("a  b"), (TokenVec{{1, K::Identifier}, {2, K::SpaceTrivia}, {1, K::Identifier}, {0, K::Eof}}));
}

TEST(ReferenceLexer, LineComments) {
    EXPECT_EQ(inv_ref("// hi"), (TokenVec{{5, K::CommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("//"), (TokenVec{{2, K::CommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("///"), (TokenVec{{3, K::CommentTrivia}, {0, K::Eof}}));
}

TEST(ReferenceLexer, TerminatingNewLineIsNotPartOfComment) {
    EXPECT_EQ(inv_ref("// hi\nx"), (TokenVec{{5, K::CommentTrivia}, {1, K::NewlineTrivia}, {1, K::Identifier}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("x // c\ny"), (TokenVec{{1, K::Identifier}, {1, K::SpaceTrivia}, {4, K::CommentTrivia}, {1, K::NewlineTrivia}, {1, K::Identifier}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("//\n//"), (TokenVec{{2, K::CommentTrivia}, {1, K::NewlineTrivia}, {2, K::CommentTrivia}, {0, K::Eof}}));
}

TEST(ReferenceLexer, CarriageReturnBeforeNewlineBelongsToComment) {
    EXPECT_EQ(inv_ref("// c\r\nx"), (TokenVec{{5, K::CommentTrivia}, {1, K::NewlineTrivia}, {1, K::Identifier}, {0, K::Eof}}));
}

TEST(ReferenceLexer, LineCommentBodiesAreOpaque) {
    EXPECT_EQ(inv_ref("// \"not a string"), (TokenVec{{static_cast<vela::lex::LexPos>(sizeof("// \"not a string") - 1), K::CommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("// if == 1.5 ;"), (TokenVec{{static_cast<vela::lex::LexPos>(sizeof("// if == 1.5 ;") - 1), K::CommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("// \xff\x80:?@#\\ /* "), (TokenVec{{static_cast<vela::lex::LexPos>(sizeof("// \xff\x80:?@#\\ /* ") - 1), K::CommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref(std::string_view("//\0x", 4)), (TokenVec{{4, K::CommentTrivia}, {0, K::Eof}}));

    // A trailing backslash does not continue the comment onto the next line.
    EXPECT_EQ(kinds("// c\\\nx"), (KindsVec{K::CommentTrivia, K::NewlineTrivia, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, BlockComments) {
    EXPECT_EQ(inv_ref("/* c */"), (TokenVec{{7, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/**/"), (TokenVec{{4, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/***/"), (TokenVec{{5, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/* * / */"), (TokenVec{{9, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/* a\nb */ x"), (TokenVec{{static_cast<vela::lex::LexPos>(sizeof("/* a\nb */") - 1), K::MultilineCommentTrivia}, {1, K::SpaceTrivia}, {1, K::Identifier}, {0, K::Eof}}));
    EXPECT_EQ(kinds("x/*c*/y"), (KindsVec{K::Identifier, K::MultilineCommentTrivia, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("a/**/b"), (KindsVec{K::Identifier, K::MultilineCommentTrivia, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, BlockCommentEdgeCases) {
    // Opening '/*' cannot double as '*/'.
    EXPECT_EQ(inv_ref("/*/"), (TokenVec{{3, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/*/ x */"), (TokenVec{{8, K::MultilineCommentTrivia}, {0, K::Eof}}));

    // "*/" outside a comment is just star then slash.
    EXPECT_EQ(kinds("*/"), (KindsVec{K::Star, K::Slash, K::Eof}));

    // Block comments dont nest.
    EXPECT_EQ(kinds("/* /* */ */"), (KindsVec{K::MultilineCommentTrivia, K::SpaceTrivia, K::Star, K::Slash, K::Eof}));

    // Line comment marker inside a block comment does nothing special.
    EXPECT_EQ(inv_ref("/* // */x"), (TokenVec{{static_cast<vela::lex::LexPos>(sizeof("/* // */") - 1), K::MultilineCommentTrivia}, {1, K::Identifier}, {0, K::Eof}}));

    // Block comment marker inside line comment does nothing special.
    EXPECT_EQ(kinds("// /* \nx */"), (KindsVec{K::CommentTrivia, K::NewlineTrivia, K::Identifier, K::SpaceTrivia, K::Star, K::Slash, K::Eof}));
}

TEST(ReferenceLexer, UnterminatedBlockCommentsRunToEof) {
    EXPECT_EQ(inv_ref("/*"), (TokenVec{{2, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/* abc"), (TokenVec{{6, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("/* abc *"), (TokenVec{{8, K::MultilineCommentTrivia}, {0, K::Eof}}));
    EXPECT_EQ(inv_ref("x /* a\n\"b"), (TokenVec{{1, K::Identifier}, {1, K::SpaceTrivia}, {static_cast<vela::lex::LexPos>(sizeof("/* a\n\"b") - 1), K::MultilineCommentTrivia}, {0, K::Eof}}));
}

TEST(ReferenceLex, SlashIsStillDivisionOperatorOutsideOfCommentContext) {
    EXPECT_EQ(inv_ref("/"), (TokenVec{{1, K::Slash}, {0, K::Eof}}));
    EXPECT_EQ(kinds("a/b"), (KindsVec{K::Identifier, K::Slash, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("a / b"), (KindsVec{K::Identifier, K::SpaceTrivia, K::Slash, K::SpaceTrivia, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("/ /"), (KindsVec{K::Slash, K::SpaceTrivia, K::Slash, K::Eof}));
    EXPECT_EQ(kinds("a /b"), (KindsVec{K::Identifier, K::SpaceTrivia, K::Slash, K::Identifier, K::Eof}));

    // '/' is not part of any two byte operator.
    EXPECT_EQ(kinds("/="), (KindsVec{K::Slash, K::Assign, K::Eof}));
    EXPECT_EQ(kinds("a /\n/ b"), (KindsVec{K::Identifier, K::SpaceTrivia, K::Slash, K::NewlineTrivia, K::Slash, K::SpaceTrivia, K::Identifier, K::Eof}));
}

TEST(ReferenceLexer, CommentsAdjacentToOtherTokens) {
    EXPECT_EQ(kinds("\"a\"//c"), (KindsVec{K::StringLiteral, K::CommentTrivia, K::Eof}));
    EXPECT_EQ(kinds("\"a\"/*c*/\"b\""), (KindsVec{K::StringLiteral, K::MultilineCommentTrivia, K::StringLiteral, K::Eof}));
    EXPECT_EQ(kinds("a<=/*c*/b"), (KindsVec{K::Identifier, K::LtEq, K::MultilineCommentTrivia, K::Identifier, K::Eof}));
    EXPECT_EQ(kinds("=/**/="), (KindsVec{K::Assign, K::MultilineCommentTrivia, K::Assign, K::Eof}));
    EXPECT_EQ(kinds("1/**/2"), (KindsVec{K::IntLiteral, K::MultilineCommentTrivia, K::IntLiteral, K::Eof}));
}

TEST(ReferenceLexer, CommentEndsNumberOrIdentifierRun) {
    EXPECT_EQ(kinds("12//ab"), (KindsVec{K::IntLiteral, K::CommentTrivia, K::Eof}));
    EXPECT_EQ(kinds("ab//12"), (KindsVec{K::Identifier, K::CommentTrivia, K::Eof}));
    EXPECT_EQ(kinds("12.3//ab"), (KindsVec{K::FloatLiteral, K::CommentTrivia, K::Eof}));
    EXPECT_EQ(kinds("12.//12"), (KindsVec{K::IntLiteral, K::Dot, K::CommentTrivia, K::Eof}));
}

TEST(ReferenceLexer, CorrectlyLexesDifferentialTestCases) {
    for (const std::string_view view : kDifferentialTestCases) {
        EXPECT_EQ(malformed(view, inv_ref(view)), "") << "input: \"" << view << "\"";
    }
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

TEST(Differential, EveryBetweenLetters) {
    for (const std::string_view ctx : {"a%cb", "1%c2", "\"x%cy\"", "= %c ="}) {
        for (int i = 0; i < 256; ++i) {
            std::string src(ctx);
            src.replace(src.find("%c"), 2, 1, static_cast<char>(i));
            ASSERT_TRUE(same_as_reference(src)) << "byte=" << i << " context=" << ctx;
        }
    }
}

TEST(Differential, Trivia) {
    for (const std::string_view view : {" ", "  ", "\t", "\r\n", " \t \t", "\n\n  \n", "a b", "a  b", "a\t\tb", "a\r\n\r\nb"}) {
        EXPECT_TRUE(same_as_reference(view)) << "input: \"" << view << "\"";
    }
}

TEST(Differential, CommentsAcrossBlockBoundaries) {
    for (std::size_t body = 0; body <= 3 * sizeof(backend::MaskType) + 2; ++body) {
        for (std::size_t shift = 0; shift < sizeof(backend::MaskType); ++shift) {
            const std::string pad(shift, ' ');
            ASSERT_TRUE(same_as_reference(pad + build_line_comment_cross_block(body))) << "line body=" << body << " shift=" << shift;
            ASSERT_TRUE(same_as_reference(pad + block_comment_cross_block(body))) << "block body=" << body << " shift=" << shift;
            ASSERT_TRUE(same_as_reference(pad + block_comment_with_quote_cross_block(body))) << "quote body=" << body << " shift=" << shift;
            ASSERT_TRUE(same_as_reference(pad + string_with_comment_markers_cross_block(body))) << "string body=" << body << " shift=" << shift;
        }
    }
}

TEST(Differential, UnterminatedBlockCommentAtEveryAlignment) {
    for (std::size_t shift = 0; shift < 2 * sizeof(backend::MaskType); ++shift) {
        const std::string pad(shift, ' ');
        ASSERT_TRUE(same_as_reference(pad + "/*")) << "shift=" << shift;
        ASSERT_TRUE(same_as_reference(pad + "/* x")) << "shift=" << shift;
        ASSERT_TRUE(same_as_reference(pad + "//")) << "shift=" << shift;
        ASSERT_TRUE(same_as_reference(pad + "// x")) << "shift=" << shift;
    }
}

// We can address this later
/*
TEST(Differential, ExhaustiveShortOverOperatorAlphabet) {
    constexpr static std::string_view kAlphabet = "=<>!&|\"\\a1. ;";
    constexpr static int kMaxLen = 5;

    std::string src;
    auto recurse = [&](auto &&self, const int depth) -> void {
        ASSERT_TRUE(same_as_reference(src));
        if (depth == kMaxLen) {
            return;
        }

        for (const char c : kAlphabet) {
            src.push_back(c);
            self(self, depth + 1);
            src.pop_back();

            if (::testing::Test::HasFailure()) {
                return;
            }
        }
    };
    recurse(recurse, 0);
}
*/