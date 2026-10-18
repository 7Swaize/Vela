#ifndef VELA_SELECT_H
#define VELA_SELECT_H

// clang-format off
#if defined(VELA_SUPPORTS_AVX2)
    #include "avx2.h"
    namespace vela::lex { namespace backend = avx2; }
#elif defined(VELA_SUPPORTS_NEON)
    #include "neon.h"
    namespace vela::lex { namespace backend = neon; }
#endif
// clang-format on

#endif // VELA_SELECT_H
