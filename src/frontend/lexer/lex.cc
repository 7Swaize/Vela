// ReSharper disable CppLocalVariableMayBeConst
#include <bit>
#include <limits>
#include <vector>

#include "backend/block.h"
#include "backend/select.h"
#include "keywords_gen.h"
#include "lex.h"
#include "token.h"

namespace vela::lex {
    constexpr unsigned int kMaskWidth = sizeof(backend::MaskType) * 8;
    constexpr unsigned int kMaskWidthShift = std::countr_zero(kMaskWidth);

    constexpr LexPos NPOS = static_cast<LexPos>(-1);

    namespace {
        struct Stage1 {
            std::vector<LexPos> starts; // byte offsets of token starts
            std::vector<backend::MaskType> ident, real_quotes_mask, pair; // per-block masks kept for stage 2
        };
    }

    static void stage1(const uint8_t *source, const LexPos nBlocks, Stage1 &stage) {
        BlockState<backend::MaskType> state{};
        BlockMasks<backend::MaskType> masks{};

        for (LexPos i = 0; i < nBlocks; ++i) {
            backend::classify_block(source + kMaskWidth * i, state, masks);

            stage.ident.push_back(masks.ident);
            stage.real_quotes_mask.push_back(masks.real_quotes);
            stage.pair.push_back(masks.pair);

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
            unsigned offset = pos & (kMaskWidth - 1); // pos % kMaskWidth
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
    static LexPos next_set(const std::vector<backend::MaskType> &masks, const LexPos pos) {
        LexPos word = pos >> kMaskWidthShift;

        if (word >= masks.size()) {
            return NPOS;
        }

        backend::MaskType x = masks[word] & (std::numeric_limits<backend::MaskType>::max() << (pos & (kMaskWidth - 1))); // clears bits below pos
        while (!x) {
            if (++word >= masks.size()) {
                return NPOS;
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

    static VELA_FORCE_INLINE bool is_pair(const uint8_t a, const uint8_t b) {
        return ((a == '=' || a == '<' || a == '>' || a == '!') && b == '=')
               || (a == '&' && b == '&')
               || (a == '|' && b == '|')
               || (a == '<' && b == '<')
               || (a == '>' && b == '>');
    }

    // Should compile to a lookup table
    // Clang may compile to a jump table, so we should force a lookup table in that event
    static TokenKind classify_single_kind(const uint8_t c) {
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

    static Token lex_string(const Stage1 &stage, const LexPos pos, const LexPos n) {
        LexPos close = next_set(stage.real_quotes_mask, pos + 1);
        const bool terminated = close != NPOS && close < n;

        return {
            .start = pos,
            .end = close + 1,
            .kind = terminated ? TokenKind::StringLiteral : TokenKind::Error
        };
    }

    static Token lex_word(const uint8_t *source, const Stage1 &stage, const LexPos pos) {
        LexPos end = pos + run_len(stage.ident, pos);

        if (!is_digit(source[pos])) {
            return {
                .start = pos,
                .end = end,
                .kind = keyword_or_ident(source + pos, end - pos)
            };
        }

        TokenKind kind = TokenKind::IntLiteral;
        if (source[end] == '.' && is_digit(source[end + 1])) {
            end += 1 + run_len(stage.ident, end + 1);
            kind = TokenKind::FloatLiteral;
        }

        return {.start = pos, .end = end, .kind = kind};
    }

    static Token lex_pair(const uint8_t *source, const LexPos pos) {
        // A pair directly followed by another pair (e.g. "===") is malformed.
        if (is_pair(source[pos + 1], source[pos + 2])) {
            return {.start = pos, .end = pos + 1, .kind = TokenKind::Error};
        }
        return {
            .start = pos,
            .end = pos + 2,
            .kind = classify_pair_kind(source[pos], source[pos + 1])
        };
    }

    static Token lex_token(const uint8_t *source, const LexPos n, const Stage1 &stage, const LexPos pos) {
        if (bit_set(stage.real_quotes_mask, pos)) return lex_string(stage, pos, n);
        if (bit_set(stage.ident, pos)) return lex_word(source, stage, pos);
        if (bit_set(stage.pair, pos)) return lex_pair(source, pos);

        return {
            .start = pos,
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

            const Token token = lex_token(source, n, stage, pos);
            tokens.push_back(token);
            cursor = token.end;
        }

        tokens.push_back({.start = n, .end = n, .kind = TokenKind::Eof});
        return tokens;
    }

    std::vector<Token> lex(const shared::FileSourceBuffer<backend::MaskType>& src) {
        const auto nBlocks = static_cast<LexPos>(src.alignedSize() / sizeof(backend::MaskType));

        Stage1 stage{};
        stage1(src.data(), nBlocks, stage);
        return stage2(src.data(), static_cast<LexPos>(src.rawSize()), stage);
    }
} // namespace vela::lex