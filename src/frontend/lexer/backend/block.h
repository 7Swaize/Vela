#ifndef VELA_BLOCK_H
#define VELA_BLOCK_H

#include <utility>

#include "../token.h"
#include "shared/typing.h"

namespace vela::lex {
    constexpr inline uint8_t kIndentBits = 0x0F;
    constexpr inline uint8_t kPunctBits = 0x70;
    constexpr inline uint8_t kDigitBits = 0x08;

    enum ExecutionContext : uint8_t {
        Code,
        String,
        SingleLineComment,
        MultiLineComment
    };

    template <shared::FixedUnsignedInteger T>
    struct BlockState {
        T escape_carry = 0;
        T pair_carry = 0;
        T ident_carry = 0;
        ExecutionContext context = ExecutionContext::Code;
    };

    template <shared::FixedUnsignedInteger T>
    struct BlockMasks {
        T ident, punct, digits, ws, quote, bslash, escaped, real_quotes,
                in_str, pair, second, str_start, real_comment, cstarts, bad;
        LexPos starts;
    };

    // I don't want to go through the work a vectorized mask creation process right now.
    // Therefore, I have this scalar version currently.
    template <typename T>
    void resolve_comment_string_mask_scalar(const uint8_t *p, T quotes, BlockState<T> &state, T &in_str, T &comment, T &cstarts) {
        constexpr unsigned int Width = sizeof(T) * 8;
        constexpr auto bit = [](unsigned int i) {
            return static_cast<T>(T{1} << i);
        };

        for (int i = 0; i < Width; i++) {
            const uint8_t curr = p[i];
            const uint8_t next = p[i + 1]; // we are guaranteed this is valid

            switch (state.context) {
                case Code:
                    if (quotes & bit(i)) {
                        in_str |= bit(i);
                        state.context = ExecutionContext::String;
                    } else if (curr == '/' && (next == '/' || next == '*')) {
                        cstarts |= bit(i);
                        comment |= bit(i) | bit(i + 1);
                        state = next == '/' ? ExecutionContext::SingleLineComment : ExecutionContext::MultiLineComment;
                        ++i;
                    }
                    break;
                case String:
                    if (quotes & bit(i)) {
                        state.context = ExecutionContext::Code;
                    } else {
                        in_str |= bit(i);
                    }
                    break;
                case SingleLineComment:
                    if (curr == '\n') {
                        state.context = ExecutionContext::Code;
                    } else {
                        comment |= bit(i);
                    }
                    break;
                case MultiLineComment:
                    comment |= bit(i);
                    if (curr == '*' && next == '/') {
                        state.context = ExecutionContext::Code;
                        ++i;
                        comment |= bit(i);
                    }
                    break;
                default:
                    std::unreachable();
            }
        }
    }
} // namespace vela::lex

#endif // VELA_BLOCK_H
