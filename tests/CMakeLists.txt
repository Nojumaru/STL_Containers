include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(vector_test vector_test.cpp)

target_link_libraries(vector_test PRIVATE nojo GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vector_test)