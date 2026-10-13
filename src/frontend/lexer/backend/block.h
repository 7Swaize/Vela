#ifndef VELA_BLOCK_H
#define VELA_BLOCK_H

#include "../token.h"
#include "shared/typing.h"

namespace vela::lex {
    constexpr inline uint8_t kIndentBits = 0x0F;
    constexpr inline uint8_t kPunctBits = 0x70;

    template <FixedUnsignedInteger T>
    struct BlockState {
        T escape_carry = 0;
        T pair_carry = 0;
        T ident_carry = 0;
        T in_string = 0; // If we are in a string we need to xor every bit with 1 to flip outcome of prefix_xor
    };

    template <FixedUnsignedInteger T>
    struct BlockMasks {
        T ident, punct, ws, quote, bslash, escaped, real_quotes,
                in_str, pair, second, str_start, bad;
        LexPos starts;
    };
} // namespace vela::lex

#endif // VELA_BLOCK_H
