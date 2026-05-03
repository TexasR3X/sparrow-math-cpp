cmake_minimum_required(VERSION 3.15)
project(sparrow_math VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(sparrow_math
    src/sparrow_math.cpp
    src/internal/ParsingUtils/TokenUtils.cpp
)

target_include_directories(sparrow_math
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)
