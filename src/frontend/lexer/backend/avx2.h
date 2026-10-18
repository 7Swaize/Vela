// ReSharper disable CppLocalVariableMayBeConst
#ifndef VELA_AVX2_H
#define VELA_AVX2_H

#if !defined(VELA_SUPPORTS_AVX2)
  #error "AVX2 dependent header included without AVX2 enabled"
#endif

#include <bit>
#include <immintrin.h>

#include "block.h"
#include "frontend/lexer/table_gen.h"
#include "shared/preprocessor.h"


namespace vela::lex::avx2 {
    using MaskType = uint32_t;

    // clang-format off
    VELA_FORCE_INLINE uint32_t movemask(const __m256i v) noexcept { return static_cast<uint32_t>(_mm256_movemask_epi8(v)); }
    VELA_FORCE_INLINE uint32_t eq(const __m256i v, const char c) noexcept { return movemask(_mm256_cmpeq_epi8(v, _mm256_set1_epi8(c))); }

    VELA_FORCE_INLINE uint32_t any_bits(const __m256i cls, const uint8_t bits) noexcept {
        // "is (cls & bits) != 0" per byte
        const __m256i inter = _mm256_and_si256(cls, _mm256_set1_epi8(static_cast<char>(bits)));
        return ~movemask(_mm256_cmpeq_epi8(inter, _mm256_setzero_si256()));
    }

    VELA_FORCE_INLINE uint32_t prefix_xor_dw(const uint32_t qbv) noexcept {
        const __m128i ones = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
        const __m128i inter = _mm_clmulepi64_si128(_mm_cvtsi32_si128(static_cast<int>(qbv)), ones, 0);
        return static_cast<uint32_t>(_mm_cvtsi128_si32(inter));
    }
    // clang-format on

    inline uint32_t find_escaped(const uint32_t backslashes, uint32_t &carry) {
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

    inline void classify_block(const uint8_t *p, BlockState<uint32_t> &state, BlockMasks<uint32_t> &masks) {
        const __m256i LO = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i *>(kTables.low.data())));
        const __m256i HI = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i *>(kTables.high.data())));
        const __m256i WS = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i *>(kTables.ws.data())));
        const __m256i NIB = _mm256_set1_epi8(0x0F);

        __m256i in = _mm256_load_si256(reinterpret_cast<const __m256i *>(p)); // Source buffer is aligned to masktype boundary, so we can do an aligned read here
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
        state.in_string = static_cast<uint32_t>(static_cast<int32_t>(in_str) >> 31);

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
} // namespace vela::lex::avx2

#endif // VELA_AVX2_H
