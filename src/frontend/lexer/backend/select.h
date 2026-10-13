#ifndef VELA_SELECT_H
#define VELA_SELECT_H

// clang-format off
#if defined(__AVX2__)
    #include "avx2.h"
    namespace vela::lex { namespace backend = avx2; }
#elif defined(__ARM_NEON)
    #include "neon.h"
    namespace vela::lex { namespace backend = neon; }
#endif
// clang-format on

#endif // VELA_SELECT_H
