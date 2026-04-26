find_package(GTest REQUIRED)

add_executable(test_sparrow_math test_sparrow_math.cpp)

target_link_libraries(test_sparrow_math
    PRIVATE
    sparrow_math
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_sparrow_math)
