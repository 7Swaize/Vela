#ifndef VELA_NEON_H
#define VELA_NEON_H

#if !defined(__ARM_NEON)
  #error "ARM NEON dependent header included without ARM NEON enabled"
#endif

#include "block.h"
#include "shared/preprocessor.h"

namespace vela::lex::neon {
    using MaskType = uint16_t;

    inline void classify_block(const uint8_t *p, BlockState<uint16_t> &state, BlockMasks<uint16_t> &masks) { }
} // namespace vela::lex:neon

#endif // VELA_NEON_H
