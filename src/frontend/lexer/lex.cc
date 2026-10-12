// ReSharper disable CppLocalVariableMayBeConst
#include <cstdint>
#include <immintrin.h>
#include <vector>

#include "keywords_gen.h"
#include "table_gen.h"
#include "token.h"
#include "shared/preprocessor.h"

namespace vela::lex {
    constexpr inline uint8_t kIndentBits = 0x0F;
    constexpr inline uint8_t kPunctBits = 0x70;

    constexpr inline LexPos NPOS = static_cast<LexPos>(-1);

    struct BlockState {
        uint32_t escape_carry = 0;
        uint32_t pair_carry = 0;
        uint32_t ident_carry = 0;
        uint32_t in_string = 0;
    };

    struct BlockMasks {
        uint32_t ident, punct, ws, quote, bslash, escaped, real_quotes,
                in_str, pair, second, str_start, bad;
        LexPos starts;
    };

    static VELA_FORCE_INLINE uint32_t movemask(const __m256i v) noexcept { return static_cast<uint32_t>(_mm256_movemask_epi8(v)); }
    static VELA_FORCE_INLINE uint32_t eq(const __m256i v, const char c) noexcept { return movemask(_mm256_cmpeq_epi8(v, _mm256_set1_epi8(c))); }

    static VELA_FORCE_INLINE uint32_t any_bits(const __m256i cls, const uint8_t bits) noexcept {
        // "is (cls & bits) != 0" per byte
        const __m256i inter = _mm256_and_si256(cls, _mm256_set1_epi8(static_cast<char>(bits)));
        return ~movemask(_mm256_cmpeq_epi8(inter, _mm256_setzero_si256()));
    }

    static VELA_FORCE_INLINE uint32_t prefix_xor_dw(const uint32_t qbv) noexcept {
        const __m128i ones = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
        const __m128i inter = _mm_clmulepi64_si128(_mm_cvtsi32_si128(static_cast<int>(qbv)), ones, 0);
        return static_cast<uint32_t>(_mm_cvtsi128_si32(inter));
    }

    static uint32_t find_escaped(const uint32_t backslashes, uint32_t &carry) {
        uint32_t escaped = carry;
        carry = 0;
        uint32_t todo = backslashes & ~escaped;

        while (todo) {
            const auto i = static_cast<unsigned int>(std::countr_zero(todo));

            if (i == 31) {
                carry = 1;
                break;
            }

            escaped |= 1u << (i + 1); // sets the next bit as escaped
            todo &= ~((1u << i) | (2u << i)); // skips second backslash in pair
        }

        return escaped;
    }

    static void classify_block(const uint8_t *p, BlockState &state, BlockMasks &masks) {
        const __m256i LO = _mm256_broadcastsi128_si256(_mm_loadu_si128(reinterpret_cast<const __m128i *>(kTables.low.data())));
        const __m256i HI = _mm256_broadcastsi128_si256(_mm_loadu_si128(reinterpret_cast<const __m128i *>(kTables.high.data())));
        const __m256i WS = _mm256_broadcastsi128_si256(_mm_loadu_si128(reinterpret_cast<const __m128i *>(kTables.ws.data())));
        const __m256i NIB = _mm256_set1_epi8(0x0F);

        __m256i in = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(p));
        __m256i next = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(p + 1));

        __m256i hi = _mm256_and_si256(_mm256_srli_epi16(in, 4), NIB);
        __m256i lo = in; // highest bit is always 0 for ASCII (no extension) char set
        __m256i cls = _mm256_and_si256(_mm256_shuffle_epi8(HI, hi), _mm256_shuffle_epi8(LO, lo));

        uint32_t ident = any_bits(cls, kIndentBits);
        uint32_t punct = any_bits(cls, kPunctBits);
        uint32_t ws = movemask(_mm256_cmpeq_epi8(_mm256_shuffle_epi8(WS, in), in));
        uint32_t quote = eq(in, '"');
        uint32_t bs = eq(in, '\\');

        // Strings
        uint32_t escaped = find_escaped(bs, state.escape_carry);
        uint32_t real_quotes = quote & ~escaped;
        uint32_t in_str = prefix_xor_dw(real_quotes) ^ state.in_string; // 1 from opening quote up to (not including) closing quote
        uint32_t outside = ~in_str;
        ident &= outside;
        punct &= outside;
        uint32_t str_start = real_quotes & in_str;
        uint32_t bad = ~(ident | punct | ws | real_quotes) & outside; // bytes Vela doesn't know

        // Two character operators
        uint32_t c_eq = eq(in, '='), c_lt = eq(in, '<'), c_gt = eq(in, '>'), c_ex = eq(in, '!');
        uint32_t c_am = eq(in, '&'), c_pi = eq(in, '|');
        uint32_t n_eq = eq(next, '='), n_lt = eq(next, '<'), n_gt = eq(next, '>'), n_am = eq(next, '&'), n_pi = eq(next, '|');
        uint32_t pair = ((c_eq | c_lt | c_gt | c_ex) & n_eq) | (c_am & n_am) | (c_pi & n_pi) | (c_lt & n_lt) | (c_gt & n_gt);
        pair &= punct;

        uint32_t second = (pair << 1) | state.pair_carry;
        state.pair_carry = pair >> 31;

        // Token starts
        uint32_t prev_ident = (ident << 1) | state.ident_carry;
        state.ident_carry = ident >> 31;
        LexPos starts = (ident & ~prev_ident) | (punct & ~second) | str_start | bad;

        masks = {
            .ident = ident,
            .punct = punct,
            .ws = ws,
            .quote = quote,
            .bslash = bs,
            .escaped = escaped,
            .real_quotes = real_quotes,
            .in_str = in_str,
            .pair = pair,
            .second = second,
            .str_start = str_start,
            .bad = bad,
            .starts = starts
        };
    }

    struct Stage1 {
        std::vector<LexPos> starts; // byte offsets of token starts
        std::vector<uint32_t> ident, real_quotes_mask, pair; // per-block masks kept for stage 2
    };

    static void stage1(const uint8_t *source, const LexPos nBlocks, Stage1 &stage) {
        BlockState state{};
        BlockMasks masks{};

        for (int i = 0; i < nBlocks; ++i) {
            classify_block(source + 32 * i, state, masks);

            stage.ident.push_back(masks.ident);
            stage.real_quotes_mask.push_back(masks.real_quotes);
            stage.pair.push_back(masks.pair);

            LexPos x = masks.starts, base = static_cast<LexPos>(32 * i);
            while (x) {
                stage.starts.push_back(base + static_cast<LexPos>(std::countr_zero(x)));
                x &= x - 1; // clears rightmost bit
            }
        }
    }

    // Counts number of consecutive ones (crosses block boundaries)
    static LexPos run_len(const std::vector<uint32_t> &masks, LexPos pos) {
        LexPos len = 0;

        while (pos >> 5 < masks.size()) {
            unsigned offset = pos & 31; // pos % 32
            uint32_t target = ~(masks[pos >> 5] >> offset);

            if (unsigned int ones = std::countr_zero(target); ones < 32 - offset) {
                return len + ones;
            }

            len += 32 - offset;
            pos += 32 - offset;
        }

        return len;
    }

    // First 1 bit at or after 'pos'
    static LexPos next_set(const std::vector<uint32_t> &masks, const LexPos pos) {
        LexPos word = pos >> 5;

        if (word >= masks.size()) {
            return NPOS;
        }

        uint32_t x = masks[word] & (~0u << (pos & 31)); // clears bits below pos
        while (!x) {
            if (++word >= masks.size()) {
                return NPOS;
            }

            x = masks[word];
        }

        return word * 32 + std::countr_zero(x);
    }

    static TokenKind keyword_or_ident(const uint8_t *source, const LexPos len) {
        if (len < kShortestKeyword || len > kLongestKeyword) {
            return TokenKind::Identifier;
        }

        const PerfectKeywordHashTableEntry* entry = VelaKeywords::lookup(reinterpret_cast<const char *>(source), len);
        return entry != nullptr ? entry->kind : TokenKind::Identifier;
    }

    static VELA_FORCE_INLINE bool bit_set(const std::vector<uint32_t> &masks, const LexPos pos) {
        return masks[pos >> 5] & (1 << (pos & 31));
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
            .end = close,
            .kind = terminated ? TokenKind::Error : TokenKind::Identifier
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
        std::vector<Token> tokens;
        tokens.reserve(stage.starts.size() + 1);

        LexPos cursor = 0;
        for (const LexPos pos : stage.starts) {
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

    static std::vector<Token> lex(const std::string_view src) {
        LexPos size = src.size();
        LexPos nBlocks = (size + 31) / 32;
    }

} // namespace vela::lex
