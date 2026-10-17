#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#if defined(_MSC_VER)
    #define VELA_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define VELA_FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define VELA_FORCE_INLINE inline
#endif


#if defined(_MSC_VER)
    #define VELA_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
    #define VELA_RESTRICT __restrict__
#else
    #define VELA_RESTRICT
#endif


#define VELA_LIKELY(x) __builtin_expect(!!(x), 1)
#define VELA_UNLIKELY(x) __builtin_expect(!!(x), 0)

#endif // PREPROCESSOR_H
