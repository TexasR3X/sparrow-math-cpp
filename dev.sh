#!/bin/bash
set -e

# Force this script to run in the right directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SCRIPT_DIR
cd "$SCRIPT_DIR"

# Make/move into the `build/` folder
mkdir -p build
cd build

# Install the Conan dependencies
conan install .. --build=missing

# Configure CMake the Conan toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

# Build the unit test executable
cmake --build .

# Move to the root directory
cd ..

# Run the unit test executable
./build/tests/test_sparrow_math
