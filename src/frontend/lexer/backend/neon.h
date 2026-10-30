// ReSharper disable CppLocalVariableMayBeConst
#ifndef VELA_NEON_H
#define VELA_NEON_H

#if !defined(VELA_SUPPORTS_NEON)
  #error "ARM NEON dependent header included without ARM NEON enabled"
#endif

#include <arm_neon.h>
#include <bit>

#include "block.h"
#include "frontend/lexer/table_gen.h"
#include "shared/preprocessor.h"

namespace vela::lex::neon {
    using MaskType = uint16_t;
    using SimdWord = uint16x8_t;

    // clang-format off
    VELA_FORCE_INLINE uint8x16_t vpshub(const uint8x16_t table, const uint8x16_t indices) noexcept {
        const uint8x16_t NIB = vdupq_n_u8(0x0F);
        return vqtbl1q_u8(table, vandq_u8(indices, NIB)); // assuming the MSB is always 0, we can ignore special behaviour
    }

    // Implementation assumes each 8-bit lane is either 0x00 or 0xFF.
    // See ref: https://developer.arm.com/community/arm-community-blogs/b/servers-and-cloud-computing-blog/posts/porting-x86-vector-bitmask-optimizations-to-arm-neon
    VELA_FORCE_INLINE uint16_t movemask(const uint8x16_t v) noexcept {
        alignas(16) static constexpr uint8_t bitmask[16] = {
            0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80
        };

        uint8x16_t imm = vandq_u8(v, vld1q_u8(bitmask));
        imm = vpaddq_u8(imm, imm);
        imm = vpaddq_u8(imm, imm);
        imm = vpaddq_u8(imm, imm);

        return vgetq_lane_u16(vreinterpretq_u16_u8(imm), 0);
    }

    VELA_FORCE_INLINE uint16_t eq(const uint8x16_t v, const unsigned char c) noexcept {
        return movemask(vceqq_u8(v, vdupq_n_u8(c)));
    }

    VELA_FORCE_INLINE uint16_t any_bits(const uint8x16_t cls, const uint8_t bits) {
        // "is (cls & bits) != 0" per byte
        const uint8x16_t inter = vandq_u8(cls, vdupq_n_u8(bits));
        return ~movemask(vceqq_u8(inter, vdupq_n_u8(0)));
    }

    VELA_FORCE_INLINE uint16_t prefix_xor_hw(const uint16_t qbv) {
        const poly128_t inter = vmull_p64(qbv, 0XFFFFFFFFFFFFFFFFull);
        return vgetq_lane_u16(vreinterpretq_u16_p16(vreinterpretq_p16_p128(inter)), 0);
    }
    // clang-format on

    inline uint16_t find_escaped(const uint16_t backslashes, uint16_t &carry) {
        uint16_t escaped = carry;
        carry = 0;
        uint16_t todo = backslashes & ~escaped;

        while (todo) {
            const auto i = static_cast<unsigned int>(std::countr_zero(todo));

            if (i == 15) {
                carry = 1;
                break;
            }

            escaped |= 1u << (i + 1); // sets the next bit as escaped
            todo &= ~((1u << i) | (2u << i)); // skips second backslash in pair
        }

        return escaped;
    }

    inline void classify_block(const uint8_t *p, BlockState<uint16_t> &state, BlockMasks<uint16_t> &masks) {
        const uint8x16_t LO = vld1q_u8(reinterpret_cast<const uint8_t *>(kTables.low.data()));
        const uint8x16_t HI = vld1q_u8(reinterpret_cast<const uint8_t *>(kTables.high.data()));
        const uint8x16_t WS = vld1q_u8(reinterpret_cast<const uint8_t *>(kTables.ws.data()));

        uint8x16_t in = vld1q_u8(p);
        uint8x16_t next = vld1q_u8(p + 1);

        uint8x16_t hi = vshrq_n_u8(in, 4);
        uint8x16_t cls = vandq_u8(vpshub(HI, hi), vpshub(LO, in));

        uint16_t ident = any_bits(cls, kIndentBits);
        uint16_t punct = any_bits(cls, kPunctBits);
        uint16_t digits = any_bits(cls, kDigitBits);
        uint16_t ws = movemask(vceqq_u8(vpshub(WS, in), in));
        uint16_t quote = eq(in, '"');
        uint16_t bs = eq(in, '\\');

        // Strings
        uint16_t escaped = find_escaped(bs, state.escape_carry);
        uint16_t real_quotes = quote & ~escaped;
        uint16_t in_str = prefix_xor_hw(real_quotes) ^ state.in_string; // 1 from opening quote up to (not including) closing quote
        uint16_t outside = ~in_str;
        ident &= outside;
        punct &= outside;
        uint16_t str_start = real_quotes & in_str;
        uint16_t bad = ~(ident | punct | ws | real_quotes) & outside; // bytes Vela doesn't know

        // Two character operators
        uint16_t c_eq = eq(in, '='), c_lt = eq(in, '<'), c_gt = eq(in, '>'), c_ex = eq(in, '!');
        uint16_t c_am = eq(in, '&'), c_pi = eq(in, '|');
        uint16_t n_eq = eq(next, '='), n_lt = eq(next, '<'), n_gt = eq(next, '>'), n_am = eq(next, '&'), n_pi = eq(next, '|');
        uint16_t pair = ((c_eq | c_lt | c_gt | c_ex) & n_eq) | (c_am & n_am) | (c_pi & n_pi) | (c_lt & n_lt) | (c_gt & n_gt);
        pair &= punct;

        uint16_t second = (pair << 1) | state.pair_carry;
        state.pair_carry = pair >> 15;
        state.in_string = static_cast<uint16_t>(static_cast<int16_t>(in_str) >> 15);

        // Token starts
        uint16_t prev_ident = (ident << 1) | state.ident_carry;
        state.ident_carry = ident >> 15;
        LexPos starts = (ident & ~prev_ident) | (punct & ~second) | str_start | bad;

        masks = {
            .ident = ident,
            .punct = punct,
            .digits = digits,
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
} // namespace vela::lex:neon

#endif // VELA_NEON_H
