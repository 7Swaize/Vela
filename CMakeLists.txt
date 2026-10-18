cmake_minimum_required(VERSION 4.3)
project(vela LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(RequireAVX2)
include(RequireNEON)

option(VELA_BUILD_TESTS "Build Vela tests" OFF)
option(VELA_SANITIZE_ADDRESS "Enable Address Sanitizer" OFF)
option(VELA_SANITIZE_UNDEFINED "Enable UndefinedBehaviorSanitizer" OFF)
option(VELA_SANITIZE_THREAD "Enable Thread Sanitizer" OFF)

add_library(vela_compile_options INTERFACE)

if (MSVC)
    target_compile_options(vela_compile_options INTERFACE
        /W4 /permissive- /Zc:__cplusplus /utf-8

        $<$<CONFIG:Debug>:/Od /Zi /RTC1>
        $<$<CONFIG:Release>:/O2 /Ob3 /Gy /Gw /GF /GL>
    )
    target_link_options(vela_compile_options INTERFACE
        $<$<CONFIG:Release>:/OPT:REF /OPT:ICF /LTCG>
    )
else ()
    target_compile_options(vela_compile_options INTERFACE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion -Wsign-conversion
        -Wold-style-cast -Wnon-virtual-dtor -Woverloaded-virtual
        -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion
        -Wformat=2 -Wimplicit-fallthrough

        $<$<CONFIG:Debug>:-O0 -g3 -fno-omit-frame-pointer>

        $<$<CONFIG:Release>:-O3 -fomit-frame-pointer
        -ffunction-sections -fdata-sections
        -fno-plt -fvisibility=hidden -flto>

        $<$<CONFIG:RelWithDebInfo>:-fno-omit-frame-pointer>
    )
    target_link_options(vela_compile_options INTERFACE
        $<$<CONFIG:Release>:-flto>
    )

    if (VELA_SANITIZE_ADDRESS)
        target_compile_options(vela_compile_options INTERFACE -fsanitize=address)
        target_link_options(vela_compile_options INTERFACE -fsanitize=address)
    endif ()

    if (VELA_SANITIZE_UNDEFINED)
        target_compile_options(vela_compile_options INTERFACE
            -fsanitize=undefined -fno-sanitize-recover=undefined)
        target_link_options(vela_compile_options INTERFACE -fsanitize=undefined)
    endif ()

    if (VELA_SANITIZE_THREAD)
        target_compile_options(vela_compile_options INTERFACE -fsanitize=thread)
        target_link_options(vela_compile_options INTERFACE -fsanitize=thread)
    endif ()
endif ()


if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    find_avx2(vela_compile_options)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    find_neon(vela_compile_options)
else ()
    message(FATAL_ERROR "Vela requires x86-64 (AVX2) or AArch64 (NEON); got ${CMAKE_SYSTEM_PROCESSOR}.")
endif ()


add_library(vela_shared INTERFACE)
target_sources(vela_shared INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src
    FILES
        src/shared/helpers.h
        src/shared/preprocessor.h
        src/shared/typing.h
)

add_library(vela_lexer STATIC
    src/frontend/source_buffer.h

    src/frontend/lexer/lex.cc
    src/frontend/lexer/lex.h

    src/frontend/lexer/keywords_gen.h
    src/frontend/lexer/table_gen.h
    src/frontend/lexer/token.h

    src/frontend/lexer/backend/avx2.h
    src/frontend/lexer/backend/block.h
    src/frontend/lexer/backend/neon.h
    src/frontend/lexer/backend/select.h
)
target_include_directories(vela_lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(vela_lexer
    PUBLIC vela_shared
    PRIVATE vela_compile_options

)

add_library(vela INTERFACE)
target_link_libraries(vela INTERFACE vela_lexer)

if (VELA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
