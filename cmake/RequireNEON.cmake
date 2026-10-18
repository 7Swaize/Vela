macro(find_neon target)
    set(CMAKE_REQUIRED_QUIET_OLD ${CMAKE_REQUIRED_QUIET})
    include(CheckCXXSourceRuns)
    set(CMAKE_REQUIRED_FLAGS)

    check_cxx_source_runs("
        #include <arm_neon.h>
        int main() {
            uint32x4_t v = vdupq_n_u32(0);
            return 0;
        }"
    HAVE_NEON_EXTENSIONS)

    if (NOT HAVE_NEON_EXTENSIONS)
        message(FATAL_ERROR "Vela requires NEON support on ${CMAKE_SYSTEM_PROCESSOR}.")
    endif ()

    target_compile_definitions(${target} INTERFACE VELA_SUPPORTS_NEON)
    set(CMAKE_REQUIRED_QUIET ${_VELA_QUIET_OLD})
endmacro()