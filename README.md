# Sparrow Math &mdash; C++ Library

## Style Guide
### General Naming Conventions
This library uses naming conventions very similar to Microsofts naming conventions. See the quick reference table below.
| Programming Element  | Name Casing            |
| -------------------- | ---------------------- |
| Local Variables      | `camelCase`            |
| Private Data Members | `_camelCase`           |
| Public Data Members  | `PascalCase`           |
| Functions            | `PascalCase`           |
| Structures           | `PascalCase`           |
| Classes              | `PascalCase`           |
| Enums                | `PascalCase`           |
| Enum Fields          | `PascalCase`           |
| Macros               | `SCREAMING_SNAKE_CASE` |
| Namespaces           | `snake_case`           |
| Files                | `snake_case`           |
| Directories          | `snake_case`           |

In general, all variables, functions, classes, and etc. should favor being descriptive over being concise.

As far as files, namespaces, macros, types, public data members, and functions go, abbrevations should only be used if they are in the following table. In that case, abbrevations must be used.
| Word       | Abbreviation     |
| ---------- | ---------------- |
| Number     | `num`            |
| Variable   | `var`            |
| Constant   | `const`          |
| Argument   | `arg`            |
| Function   | `fn`             |
| Expression | `expr`           |
| String     | `str`            |
| Charactor  | `char`           |

Local variables, function parameters, and private data members may also use other abbrevations.

### Maintaining Portability
When naming things on the public API, keeping portability across languages in mind is essential. Eventually, I hope to build versions of this library in Python, JavaScript, TypeScript, C#, and perhaps other languages. It would be ideal if all public classes, functions, and variables could be named the same across every langauge (with name cases being the only difference). Of course, this isn't entirely practical, but portability should still be kept in mind.

### Variable Names
Local variables should be written in `camelCase`, while private data members should be in `_camelCase` (camel case with an underscore prefix). Public data members should in `PascalCase`.
```cpp
int myLocalVar = 0;
bool _myPrivateDataMember = false;
double MyPublicDataMember = 0.0;
```

Variables, whether they be local variables or data members, should be nouns. They should NEVER be verbs. Boolean variables should be questions (but not full sentences) that have boolean answers. Its strongly encouraged to prefix boolean variables with words like "is", "are", or "has".
```cpp
bool active = true;
bool isActive = true; // Preferred
```

### Function Names
Functions (and methods) should generally be verbs describing what they do. With that said, getter and setter functions should be nouns and should follow the exact same naming conventions as public data members; even if they are private, they should be in `PascalCase` and have no underscore prefix. Also, getters and setters shouldn't be prefixed with "get" or "set".
```cpp
// Good
std::string Name();
void Name(std::string name);
// Bad
std::string GetName();
void SetName(std::string name);
```

## Development Tool:
For those who are developing this library, the `dev.sh` file can be used to build, rebuild, and run the project's unit tests. Its run with the following syntax:
```bash
./dev.sh <option>
```

Each command option is given in the table below:
| Command Option | Description                                                    |
| -------------- | -------------------------------------------------------------- |
| `build-conan`  | Installs the Conan dependencies.                               |
| `build-cmake`  | Configures the CMake and Conan toolchain.                      |
| `build-cpp`    | Compiles the C++ and header files into a unit test executable. |
| `build`        | Same as using all of the other build options.                  |
| `run`          | Runs the unit test executable.                                 |
| (Default)      | Same as running both the `build-cpp` and `run` options.        |
