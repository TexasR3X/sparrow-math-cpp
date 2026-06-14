# Sparrow Math &mdash; C++ Library

## Style Guide
### General Naming Conventions
This library uses naming conventions very similar to Microsoft's naming conventions (as used in C#). See the quick reference table below:
| Programming Entity   | Name Casing            |
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

Programming entities should favor being descriptive over being concise. Local variables and function parameters may be more concise, within reason.

### Naming Abbreviations
When naming entities, if a word has an abbreviation in the table below, the abbreviation must be used:
| Word       | Abbreviation |
| ---------- | ------------ |
| Number     | `num`        |
| Variable   | `var`        |
| Constant   | `const`      |
| Argument   | `arg`        |
| Function   | `fn`         |
| Expression | `expr`       |
| String     | `str`        |
| Charactor  | `char`       |

All other abbreviations are prohibited, unless they are extremely well established. Local variables and function parameters are the exception: they may use other abbreviations as well, within reason.
```cpp
// Good
int myNum = 5;
std::string myVar = "x";
double Raise10ToExponent(int exp);

// Bad
int myNumber = 5;
std::string myVariable = "x";
double Raise10ToExp(int exp);
```

### Maintaining Portability
When naming programming entities on the public API, always consider how portable the names are across other programming languages. The plan is to eventually build versions of this library for Python, JavaScript, C#, and perhaps other languages. It would be ideal if all public entities could have the same names across these languages (with naming conventions being the only difference). Of course, this isn't always practical, but portability should always be kept in mind.

### Variables
Local variables and function parameters should be written in `camelCase`. Private data members should be in `_camelCase` (camel case with an underscore prefix), while public data members should be in `PascalCase`.
```cpp
int myLocalVar = 0;
bool _myPrivateDataMember = false;
double MyPublicDataMember = 0.0;
```

Variables, whether they be local variables or data members, should always be nouns. They should NEVER be verbs. Boolean variables should be questions (but not full sentences) that have boolean answers. It's strongly encouraged to prefix boolean variables with words like `is`, `are`, or `has`.
```cpp
bool active = true;
bool isActive = true; // Preferred
```

When declaring local variables or function parameters, use the `auto` keyword when possible. Also, make variables read-only references whenever possible, unless they are basic numeric types (including characters and enums).

### Functions
Functions should generally be verbs describing what they do. Getter and setter functions can be nouns if they use the prefixes `Get` and `Set`, respectively. All functions should be in `PascalCase`; even named lambda functions use `PascalCase`.

When declaring functions, please closely follow the use of spaces and indents in the example below:
```cpp
bool IsEqual(int x, int y) {
    return x == y;
}
```

Use the following format with lambda functions. Closely observe the use of spaces and indents:
```cpp
auto CalculateSum = [&x, &y](int z) -> double {
    return x + y + z;
};
```
The `()` should be left off if the function has no parameters. The return type should also be excluded if it can be inferred by the compiler.

### Enums
Both enums and enum members use `PascalCase`. They do NOT use `SCREAMING_SNAKE_CASE`.
```cpp
// Good
enum class Result {
    Complete,
    StillWorking,
    Error
}

// Bad
enum class Result {
    COMPLETE,
    STILL_WORKING,
    ERROR
}
```

## Development Tool:
For those who are developing this library, the `dev.sh` file can be used to build, rebuild, and run the project's unit tests. It's ran using the following syntax:
```
./dev.sh <option>
```

The `dev.sh` file can be used with several different command options. Each option is given in the table below:
| Command Option | Description                                                    |
| -------------- | -------------------------------------------------------------- |
| `build-conan`  | Installs the Conan dependencies.                               |
| `build-cmake`  | Configures the CMake and Conan toolchain.                      |
| `build-cpp`    | Compiles the C++ and header files into a unit test executable. |
| `build`        | Same as using all of the other build options.                  |
| `run`          | Runs the unit test executable.                                 |
| (Default)      | Same as running both the `build-cpp` and `run` options.        |
