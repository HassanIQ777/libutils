# libutils

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A versatile, header-only C++ utility library designed to simplify common programming tasks.

## Overview

`libutils` is a collection of single-header C++ utilities that provide easy-to-use interfaces for a variety of functionalities, from file manipulation and command-line parsing to terminal styling and performance benchmarking. It is designed to be lightweight and easy to integrate into any C++ project.

## Features

- **Benchmarking:** Measure execution time and CPU cycles.
- **Binary Cache:** Save and load data structures to/from binary files.
- **CLI Parser:** Simple and effective command-line argument parsing.
- **Color:** Stylize terminal output with colors and text modifiers.
- **File Management:** A comprehensive suite of tools for file and directory operations.
- **General Functions:** A collection of miscellaneous helper functions.
- **Logging:** A simple, level-based logging utility.
- **Random:** A powerful random number and data generation toolkit.
- **Table:** Create and display formatted text-based tables.
- **Text Editor:** A basic, in-terminal text editor component.
- **Timer:** High-precision timers for measuring code execution time.
- **Tokenizer:** Tools for splitting strings into tokens.

## Breaking Changes (Current)

- `m_`-prefixed public function names were renamed (for example `m_hasFlag` -> `hasFlag`).
- `BinaryCache::save(...)` and `BinaryCache::load(...)` now return `bool` and no longer throw.
- `File::writeToINI(...)` now returns `bool` and fails (`false`) when the target key is not found.
- `pager.hpp` has been removed from `libutils.hpp` until it is implemented.

## Installation

As `libutils` is a header-only library, you can simply copy the `src` directory into your project and include the desired headers.

For a more traditional installation, you can use the provided `Makefile`:

```bash
sudo make install
```

This will install the headers into `/usr/local/include/libutils/`.

## Building

While the library is header-only, the `Makefile` provides a command to create a static archive (`.a`) file, which can be useful for some build systems.

```bash
make
```

This will create `build/libutils.a`.

To clean the build artifacts, run:

```bash
make clean
```

## Modules in Detail

### Benchmark

Provides tools to measure the performance of your code.

**Example:**
```cpp
#include "libutils/src/Benchmark.hpp"
#include <iostream>

void some_function() {
    // ...
}

int main() {
    auto result = Benchmark::run(1000, some_function);
    std::cout << "Average time: " << result.m_average << "s\n";
    return 0;
}
```

### CLI Parser

A simple parser for command-line arguments.

**Example:**
```cpp
#include "libutils/src/CLIParser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    CLIParser parser(argc, argv);
    if (parser.hasFlag("--help")) {
        std::cout << "Usage: my_program [options]\n";
    }
    std::string output = parser.getValue("--output");
    if (!output.empty()) {
        std::cout << "Output file: " << output << "\n";
    }
    return 0;
}
```

### Color

Add color and styles to your terminal output.

**Example:**
```cpp
#include "libutils/src/color.hpp"
#include <iostream>

int main() {
    std::cout << color::fg_rgb(255, 100, 100) << "This is a custom red color!" << color::_RESET << std::endl;
    return 0;
}
```

### File

A comprehensive set of file and directory manipulation tools.

**Example:**
```cpp
#include "libutils/src/File.hpp"
#include <iostream>

int main() {
    std::vector<std::string> content = {"Hello", "World"};
    File::writefile("hello.txt", content);
    
    for(const auto& line : File::readfile("hello.txt")) {
        std::cout << line << std::endl;
    }
    return 0;
}
```

### Log

A simple and easy-to-use logger.

**Example:**
```cpp
#include "libutils/src/Log.hpp"

int main() {
    Log::setLogLevel(Log::LogLevel::log_debug);
    Log::info("This is an info message.");
    Log::warn("This is a warning.");
    Log::error("This is an error.", false); // false = don't terminate
    return 0;
}
```

### Random

Generate random numbers, UUIDs, and more.

**Example:**
```cpp
#include "libutils/src/Random.hpp"
#include <iostream>

int main() {
    std::cout << "Random number: " << Random::intValue(1, 100) << std::endl;
    std::cout << "UUID: " << Random::generateUUID(true) << std::endl;
    return 0;
}
```

### Table

Create formatted text tables.

**Example:**
```cpp
#include "libutils/src/Table.hpp"
#include <iostream>

int main() {
    Table table;
    table.setHeader("Name", "Score");
    table.addRow("Alice", 95);
    table.addRow("Bob", 88);
    std::cout << table;
    return 0;
}
```

### BinaryCache

Binary serialization helpers for trivially copyable vectors and string vectors.

**Example:**
```cpp
#include "libutils/src/BinaryCache.hpp"
#include <iostream>

int main() {
    std::vector<int> data = {1, 2, 3};
    if (!BinaryCache::save("numbers.bin", data)) {
        std::cerr << "save failed\n";
        return 1;
    }

    std::vector<int> loaded;
    if (!BinaryCache::load("numbers.bin", loaded)) {
        std::cerr << "load failed\n";
        return 1;
    }

    return 0;
}
```

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue.

## License

This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.
