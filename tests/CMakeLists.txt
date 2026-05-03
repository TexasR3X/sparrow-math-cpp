find_package(GTest REQUIRED)

add_executable(test_sparrow_math test_sparrow_math.cpp)

target_link_libraries(test_sparrow_math
    PRIVATE
    sparrow_math
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(test_sparrow_math
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(test_sparrow_math)
