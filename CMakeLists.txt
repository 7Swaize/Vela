cmake_minimum_required(VERSION 4.3)
project(vela LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif ()

option(VELA_BUILD_TESTS "Build Vela tests" OFF)

add_library(vela_shared INTERFACE)
target_sources(vela_shared INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src
    FILES
        src/shared/preprocessor.h
        src/shared/typing.h
)

add_library(vela_lexer STATIC
    src/frontend/lexer/lex.cc
    src/frontend/lexer/lex.h

    src/frontend/lexer/keywords_gen.h
    src/frontend/lexer/table_gen.h
    src/frontend/lexer/token.h
    src/frontend/lexer/lex_source_buffer.h

    src/frontend/lexer/backend/avx2.h
    src/frontend/lexer/backend/block.h
    src/frontend/lexer/backend/neon.h
    src/frontend/lexer/backend/select.h
)
target_include_directories(vela_lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(vela_lexer PUBLIC vela_shared)

add_library(vela INTERFACE)

target_link_libraries(vela INTERFACE vela_lexer)

if (VELA_BUILD_TESTS)
    enable_testing()
endif()
