include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 063de7e9578f82b369302001269680b4b1553359 # release-1.18
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # msvc
FetchContent_MakeAvailable(googletest)

add_executable(vela_lexer_tests
    unit/lexer/lexer_test.cc
    unit/lexer/reference_lexer.h
)
target_include_directories(vela_lexer_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vela_lexer_tests PRIVATE vela_lexer GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vela_lexer_tests)