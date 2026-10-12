cmake_minimum_required(VERSION 4.3)
project(vela LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif ()

add_library(vela_lexer STATIC
    src/frontend/lexer/lex.cc

    src/frontend/lexer/keywords_gen.h
    src/frontend/lexer/table_gen.h
    src/frontend/lexer/token.h
    src/frontend/lexer/lex_source_buffer.h
)
target_include_directories(vela_lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(vela INTERFACE)

target_link_libraries(vela INTERFACE vela_lexer)
