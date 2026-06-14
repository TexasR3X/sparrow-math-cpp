#!/bin/bash

# Ensure that if an error occures, the script stops
set -e

# Declare helper functions
install_conan_deps() {
    echo "~~~~~~~~~~ Installing Conan dependencies ~~~~~~~~~~"
    conan install .. --update --build=missing
}
config_cmake_and_conan() {
    echo "~~~~~~~~~~ Configuring CMake and Conan toolchain ~~~~~~~~~~"
    cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
}
build_executable() {
    echo "~~~~~~~~~~ Building the unit test executable ~~~~~~~~~~"
    cmake --build .
}
run_executable() {
    echo "~~~~~~~~~~ Running tests ~~~~~~~~~~"
    cd tests
    if [[ ! -f test_sparrow_math ]]; then
        echo $'ERROR: The executable file `./build/tests/test_sparrow_math` does not exist'
    elif [[ ! -x test_sparrow_math ]]; then
        echo $'ERROR: You either don\'t have permission to run the executable file, or its not an executable'
        echo $'ERROR: The file is named `./build/tests/test_sparrow_math`'
    else
        ./test_sparrow_math
    fi
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
elif [[ "$1" == "build-cmake" ]]; then
    config_cmake_and_conan
elif [[ "$1" == "build-cpp" ]]; then
    build_executable
elif [[ "$1" == "build" ]]; then
    install_conan_deps
    config_cmake_and_conan
    build_executable
elif [[ "$1" == "run" ]]; then
    run_executable
elif [[ -z "$1" ]]; then
    build_executable
    run_executable
else
    echo $'ERROR: Unknown option for `dev.sh`'
    echo $'ERROR: See `README.md` for command options'
    exit 1
fi
