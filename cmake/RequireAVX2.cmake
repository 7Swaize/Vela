# See ref: https://github.com/microsoft/APSI/blob/main/cmake/FindAVX.cmake

# Cryptographic instructions not associated with general computing are not included in the x86-64-v3 psABI.
# See ref: https://en.opensuse.org/X86-64_microarchitecture_levels

macro(find_avx2 target)
    set(CMAKE_REQUIRED_QUIET_OLD ${CMAKE_REQUIRED_QUIET})
    set(CMAKE_REQUIRED_FLAGS_OLD ${CMAKE_REQUIRED_FLAGS})
    set(CMAKE_REQUIRED_QUIET ON)
    include(CheckCXXSourceRuns)
    set(CMAKE_REQUIRED_FLAGS)

    if(MSVC AND MSVC_VERSION GREATER_EQUAL 1800)
        set(CMAKE_REQUIRED_FLAGS "/arch:AVX2")
    elseif(NOT MSVC)
        set(CMAKE_REQUIRED_FLAGS "-march=x86-64-v3 -mpclmul")
    endif()

    check_cxx_source_runs("
        #include <immintrin.h>
        int main()
        {
          const int src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
          int dst[8];
          __m256i a = _mm256_loadu_si256((const __m256i*)src);
          __m256i c = _mm256_add_epi32(a, a);
          _mm256_storeu_si256((__m256i*)dst, c);
          for (int i = 0; i < 8; i++) {
            if ((src[i] + src[i]) != dst[i]) return -1;
          }

          __m256 f = _mm256_set1_ps(2.0f);
          __m256 r = _mm256_fmadd_ps(f, f, f);   // FMA
          float out[8];
          _mm256_storeu_ps(out, r);
          if (out[0] != 6.0f) return -1;

          if (_pdep_u32(0xFu, 0xF0u) != 0xF0u) return -1;  // BMI2
          return 0;
        }"
    HAVE_AVX2_EXTENSIONS)

    if (NOT HAVE_AVX2_EXTENSIONS)
        message(FATAL_ERROR "Vela requires AVX2 support on ${CMAKE_SYSTEM_PROCESSOR}.")
    endif ()

    if (MSVC)
        if (HAVE_AVX2_EXTENSIONS AND MSVC_VERSION GREATER_EQUAL 1800)
            target_compile_options(${target} INTERFACE /arch:AVX2)
            target_compile_definitions(${target} INTERFACE VELA_SUPPORTS_AVX2)
        endif ()
    else ()
        if(HAVE_AVX2_EXTENSIONS)
            target_compile_options(${target} INTERFACE -march=x86-64-v3 -mpclmul)
            target_compile_definitions(${target} INTERFACE VELA_SUPPORTS_AVX2)
        endif ()
    endif ()

    set(CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_OLD})
    set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_OLD})
endmacro()