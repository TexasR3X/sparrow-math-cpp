#!/bin/bash

# Ensure that if an error occures, the script stops
set -e

# Declare helper functions
install_conan_deps() {
    # Install the Conan dependencies
    conan install .. --build=missing
}
config_cmake_and_conan() {
    # Configure CMake the Conan toolchain
    cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
}
build_executable() {
    # Build the unit test executable
    cmake --build .
}

# Force this script to run in the right directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SCRIPT_DIR
cd "$SCRIPT_DIR"

# Create the `build/` folder if it doesn't yet exist
mkdir -p build

# Move into the `build/` folder
cd build

# Compile/configure the things that need to be compiled/configured
if [[ "$1" == "build-conan" ]]; then
    install_conan_deps
    config_cmake_and_conan
    build_executable
elif [[ "$1" == "build-cmake" ]]; then
    config_cmake_and_conan
    build_executable
elif [[ -z "$1" || "$1" == "build-cpp" ]]; then
    build_executable
else
    echo "Error: Unknown arguement for dev.sh: $1"
    exit 1
fi

# Move to the `./build/tests/` directory
cd tests

# Run the unit test executable
./test_sparrow_math
