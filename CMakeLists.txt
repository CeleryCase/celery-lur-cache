cmake_minimum_required(VERSION 3.20)

project(celery-lru-cache VERSION 0.1.0 LANGUAGES CXX)

add_executable(lru_test demo.cc)


include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_lru_cache tests/test_lru_cache.cc)

target_link_libraries(
  test_lru_cache
  PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_lru_cache)
