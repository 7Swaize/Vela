// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppRedundantParentheses
#include <bit>
#include <limits>
#include <vector>

#include "backend/block.h"
#include "backend/select.h"
#include "keywords_gen.h"
#include "lex.h"
#include "token.h"

namespace vela::lex {
    constexpr unsigned int kMaskWidth = sizeof(backend::SimdWord);
    constexpr unsigned int kMaskWidthShift = std::countr_zero(kMaskWidth);

    constexpr LexPos NPOS = static_cast<LexPos>(-1);

    namespace {
        struct Stage1 {
            std::vector<LexPos> starts; // byte offsets of token starts
            std::vector<backend::MaskType> ident, digits,
                    real_quotes_mask, pair, ws, real_comment; // per-block masks kept for stage 2
        };

        // Just wraps a normal Token and adds 'end' property for propagation
        struct WrappedToken {
            LexPos width;
            LexPos end;
            TokenKind kind;
        };
    }

    static void stage1(const uint8_t *source, const LexPos nBlocks, Stage1 &stage) {
        BlockState<backend::MaskType> state{};
        BlockMasks<backend::MaskType> masks{};

        for (LexPos i = 0; i < nBlocks; ++i) {
            backend::classify_block(source + kMaskWidth * i, state, masks);

            stage.ident.push_back(masks.ident);
            stage.digits.push_back(masks.digits);
            stage.real_quotes_mask.push_back(masks.real_quotes);
            stage.pair.push_back(masks.pair);
            stage.ws.push_back(masks.ws);
            stage.real_comment.push_back(masks.real_comment);

            LexPos x = masks.starts, base = kMaskWidth * i;
            while (x) {
                stage.starts.push_back(base + static_cast<LexPos>(std::countr_zero(x)));
                x &= x - 1; // clears rightmost bit
            }
        }
    }

    // Counts number of consecutive ones (crosses block boundaries)
    static LexPos run_len(const std::vector<backend::MaskType> &masks, LexPos pos) {
        LexPos len = 0;

        while (pos >> kMaskWidthShift < masks.size()) {
            unsigned int offset = pos & (kMaskWidth - 1); // pos % kMaskWidth
            backend::MaskType target = ~(masks[pos >> kMaskWidthShift] >> offset);

            if (auto ones = static_cast<unsigned int>(std::countr_zero(target)); ones < kMaskWidth - offset) {
                return len + ones;
            }

            len += kMaskWidth - offset;
            pos += kMaskWidth - offset;
        }

        return len;
    }

    // First 1 bit at or after 'pos'
    static LexPos next_set(const std::vector<backend::MaskType> &masks, const LexPos pos, const LexPos n) {
        LexPos word = pos >> kMaskWidthShift;

        if (word >= masks.size()) {
            return n;
        }

        backend::MaskType x = masks[word] & (std::numeric_limits<backend::MaskType>::max() << (pos & (kMaskWidth - 1))); // clears bits below pos
        while (!x) {
            if (++word >= masks.size()) {
                return n;
            }

            x = masks[word];
        }

        return word * kMaskWidth + static_cast<unsigned int>(std::countr_zero(x));
    }

    static TokenKind keyword_or_ident(const uint8_t *source, const LexPos len) {
        if (len < kShortestKeyword || len > kLongestKeyword) {
            return TokenKind::Identifier;
        }

        const PerfectKeywordHashTableEntry* entry = VelaKeywords::lookup(reinterpret_cast<const char *>(source), len);
        return entry != nullptr ? entry->kind : TokenKind::Identifier;
    }

    static VELA_FORCE_INLINE bool bit_set(const std::vector<backend::MaskType> &masks, const LexPos pos) {
        return masks[pos >> kMaskWidthShift] & (backend::MaskType{1} << (pos & (kMaskWidth - 1)));
    }

    static VELA_FORCE_INLINE bool is_digit(const uint8_t c) {
        return c >= '0' && c <= '9';
    }

    // Should compile to a lookup table
    // Clang may compile to a jump table, so we should force a lookup table in that event
    static TokenKind classify_single_kind(const uint8_t c) {
        switch (c) {
#define X(ch, kind)                         \
            case ch:                        \
                return TokenKind::kind;
                    VELA_SINGLE_CHAR_TOKENS(X)
#undef X
            default:
                return TokenKind::Error;
        }
    }

    // Should compile to a lookup table
    // Clang may compile to a jump table, so we should force a lookup table in that event
    static TokenKind classify_pair_kind(const uint8_t a, const uint8_t b) {
        switch (a) {
            case '&':
                return TokenKind::AmpAmp;
            case '|':
                return TokenKind::PipePipe;
            case '=':
                return TokenKind::EqEq;
            case '!':
                return TokenKind::ExclEq;
            case '<':
                return b == '<' ? TokenKind::LtLt : TokenKind::LtEq;
            case '>':
                return b == '>' ? TokenKind::GtGt : TokenKind::GtEq;
            default:
                return TokenKind::Error;
        }
    }

    // Should compile to a lookup table
    // Clang may compile to a jump table, so we should force a lookup table in that event
    static TokenKind classify_ws_kind(const uint8_t c) {
        switch (c) {
            case ' ':
                return TokenKind::SpaceTrivia;
            case '\t':
                return TokenKind::TabTrivia;
            case '\n':
                return TokenKind::NewlineTrivia;
            case '\r':
                return TokenKind::CarriageReturnTrivia;
            default:
                return TokenKind::Error;
        }
    }

    static WrappedToken lex_ws(const uint8_t *source, const LexPos pos, const LexPos n) {
        TokenKind kind = classify_ws_kind(source[pos]);

        for (LexPos i = 1; pos + i < n; ++i) {
            if (classify_ws_kind(source[pos + i]) != kind) {
                return {
                    .width = i,
                    .end = pos + i,
                    .kind = kind,
                };
            }
        }

        return {
            .width = n - pos,
            .end = n,
            .kind = kind
        };
    }

    static WrappedToken lex_string(const Stage1 &stage, const LexPos pos, const LexPos n) {
        LexPos close = next_set(stage.real_quotes_mask, pos + 1, n);

        // Unclosed strings cause lexing to bail
        if (close == NPOS || close >= n) {
            return {
                .width = n - pos,
                .end = n,
                .kind = TokenKind::Error
            };
        }

        return {
            .width = (close + 1) - pos,
            .end = close + 1,
            .kind = TokenKind::StringLiteral
        };
    }

    static WrappedToken lex_word(const uint8_t *source, const Stage1 &stage, const LexPos pos) {
        const LexPos iend = pos + run_len(stage.ident, pos);

        if (!is_digit(source[pos])) {
            return {
                .width = iend - pos,
                .end = iend,
                .kind = keyword_or_ident(source + pos, iend - pos)
            };
        }

        // Something like "12ab" is malformed
        if (run_len(stage.digits, pos) != iend - pos) {
            return {.width = iend - pos, .end = iend, .kind = TokenKind::Error};
        }

        if (source[iend] == '.' && is_digit(source[iend + 1])) {
            const LexPos fstart = iend + 1;
            const LexPos fend = fstart + run_len(stage.ident, fstart);

            // Something like 12.1.2 is malformed
            const TokenKind kind = run_len(stage.digits, fstart) == fend - fstart
                ? TokenKind::FloatLiteral
                : TokenKind::Error;

            return {.width = fend - pos, .end = fend, .kind = kind};
        }

        return {.width = iend - pos, .end = iend, .kind = TokenKind::IntLiteral};
    }

    static WrappedToken lex_pair(const uint8_t *source, const Stage1& stage, const LexPos pos) {
        // A pair directly followed by another pair (e.g. "===") is malformed.
        if (bit_set(stage.pair, pos + 1)) {
            LexPos end = pos + run_len(stage.pair, pos) + 1;
            return {.width = end - pos, .end = end, .kind = TokenKind::Error};
        }

        return {
            .width = 2,
            .end = pos + 2,
            .kind = classify_pair_kind(source[pos], source[pos + 1])
        };
    }

    static WrappedToken lex_comment(const uint8_t *source, const Stage1 &stage, const LexPos pos, const LexPos n) {
        LexPos end = std::min(pos + run_len(stage.real_comment, pos), n);

        return {
            .width = end - pos,
            .end = end,
            .kind = source[pos + 1] == '/' ? TokenKind::CommentTrivia : TokenKind::MultilineCommentTrivia
        };
    }

    static WrappedToken lex_token(const uint8_t *source, const LexPos n, const Stage1 &stage, const LexPos pos) {
        if (bit_set(stage.ws, pos)) return lex_ws(source, pos, n);
        if (bit_set(stage.real_quotes_mask, pos)) return lex_string(stage, pos, n);
        if (bit_set(stage.ident, pos)) return lex_word(source, stage, pos);
        if (bit_set(stage.pair, pos)) return lex_pair(source, stage, pos);
        if (bit_set(stage.real_comment, pos)) return lex_comment(source, stage, pos, n);

        return {
            .width = 1,
            .end = pos + 1,
            .kind = classify_single_kind(source[pos])
        };
    }

    static std::vector<Token> stage2(const uint8_t *source, const LexPos n, const Stage1 &stage) {
        std::vector<Token> tokens{};
        tokens.reserve(stage.starts.size() + 1);

        LexPos cursor = 0;
        for (const LexPos pos : stage.starts) {
            if (VELA_UNLIKELY(pos >= n)) {
                break;
            }

            if (pos < cursor) {
                continue; // swallowed by the previous token (e.g. the "5" of "1.5")
            }

            const WrappedToken wrapped = lex_token(source, n, stage, pos);
            tokens.push_back(Token{.width = wrapped.width, .kind = wrapped.kind});
            cursor = wrapped.end;
        }

        tokens.push_back({.width = 0, .kind = TokenKind::Eof});
        return tokens;
    }

    std::vector<Token> lex(const shared::FileSourceBuffer<backend::SimdWord>& src) {
        const auto nBlocks = static_cast<LexPos>(src.alignedSize() / sizeof(backend::SimdWord));

        Stage1 stage{};
        stage1(src.data(), nBlocks, stage);
        return stage2(src.data(), static_cast<LexPos>(src.rawSize()), stage);
    }
} // namespace vela::lex