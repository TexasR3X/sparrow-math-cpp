# Sparrow Math &mdash; C++ Library

## Development Tool:
The `dev.sh` file can be used to build, rebuild, and run the project's unit tests. Its run with the following syntax:
```bash
./dev.sh <option>
```
Each command option is given in the table below:
| Command Option | Description                                                    |
| -------------- | -------------------------------------------------------------- |
| build-conan    | Installs the Conan dependencies.                               |
| build-cmake    | Configures the CMake and Conan toolchain.                      |
| build-cpp      | Compiles the C++ and header files into a unit test executable. |
| build          | Same as using all of the other build options.                  |
| run            | Runs the unit test executable.                                 |
| (Default)      | Same as running both the `build` and `run` options.            |
