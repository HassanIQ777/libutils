# libutils

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

A versatile, header-only C++ utility library designed to simplify common programming tasks.

## Overview

`libutils` is a collection of single-header C++ utilities that provide easy-to-use interfaces for a variety of functionalities, from file manipulation and command-line parsing to terminal styling and performance benchmarking. It is designed to be lightweight and easy to integrate into any C++ project.

## Features

- **Benchmarking:** Measure execution time and CPU cycles.
- **Binary:** Bit manipulation utilities for `uint32_t` values.
- **Binary Cache:** Save and load data structures to/from binary files.
- **CLI Parser:** Simple and effective command-line argument parsing.
- **Color:** Stylize terminal output with colors and text modifiers.
- **Cursor:** ANSI cursor movement, line clearing, visibility, and in-place line updates.
- **File Management:** A comprehensive suite of tools for file and directory operations.
- **General Functions:** A collection of miscellaneous helper functions.
- **Input:** Type-safe terminal input with ranges, defaults, validation loops, silent reads, and custom parsers.
- **Loading Bar:** Threaded terminal spinners and status lines.
- **Logging:** A level-based logging utility with variadic support.
- **Number Utilities:** Formatting helpers for numbers, durations, sizes, and bases.
- **Random:** A powerful random number and data generation toolkit.
- **String Utilities:** String padding, trimming, and transformation helpers.
- **Table:** Create and display formatted text-based tables.
- **Text Editor:** A basic, in-terminal text editor component.
- **Timer:** High-precision pausable timers and scoped timers.
- **Tokenizer:** Tools for splitting strings into tokens.

## Breaking Changes (Current)

- `m_`-prefixed public function names were renamed (e.g. `m_hasFlag` → `hasFlag`).
- `BinaryCache::save(...)` and `BinaryCache::load(...)` now return `bool` and no longer throw.
- `File::writeToINI(...)` now returns `bool` and fails (`false`) when the target key is not found.
- `pager.hpp` has been removed from `libutils.hpp` until it is implemented.
- `Log::error(...)` signature changed — `terminate` is now the **first** argument: `Log::error(bool terminate, message, ...)`.
- `Log::LogLevel` enum values renamed to avoid clashes with Linux macros: `log_debug` → `Debug`, `log_info` → `Info`, etc.
- `Random` was converted from a class to a **namespace** — old static method calls like `Random::intValue(...)` are gone; use `Random::getint(...)`, `Random::getdouble(...)`, etc.

## Installation

As `libutils` is a header-only library, you can simply copy the `src` directory into your project and include the desired headers.

For a more traditional installation, you can use the provided `Makefile`:

```bash
sudo make install
```

This will install the headers into `/usr/local/include/libutils/`.

## Updating

The repository includes an `update.sh` helper that refreshes the local checkout from `origin/main`:

```bash
./update.sh
```

It runs `git fetch origin` followed by `git reset --hard origin/main`, so commit or stash any local changes before using it.

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

---

## Modules in Detail

### Benchmark

Provides tools to measure the performance of your code.

**Example:**
```cpp
#include "libutils/src/Benchmark.hpp"
#include <iostream>

void some_function() { /* ... */ }

int main() {
    auto result = Benchmark::run(1000, some_function);
    std::cout << "Average time: " << result.m_average << "s\n";
}
```

---

### Binary

Bit manipulation utilities operating on `uint32_t` values.

| Method | Description |
|---|---|
| `Binary::set(num, i)` | Sets bit `i` to 1 |
| `Binary::unset(num, i)` | Sets bit `i` to 0 |
| `Binary::toggle(num, i)` | Flips bit `i` |
| `Binary::get(num, i)` | Returns bit `i` as `bool` |
| `Binary::flip(num)` | Inverts all bits (`~num`) |
| `Binary::clear()` | Returns `0` (all bits cleared) |

**Example:**
```cpp
#include "libutils/src/Binary.hpp"
#include <iostream>

int main() {
    uint32_t flags = Binary::clear();
    flags = Binary::set(flags, 0);   // set flag 0
    flags = Binary::set(flags, 3);   // set flag 3
    flags = Binary::toggle(flags, 3); // toggle flag 3 off

    std::cout << Binary::get(flags, 0) << "\n"; // 1
    std::cout << Binary::get(flags, 3) << "\n"; // 0
}
```

---

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
}
```

---

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
}
```

---

### Color

Add color and styles to your terminal output.

**Example:**
```cpp
#include "libutils/src/color.hpp"
#include <iostream>

int main() {
    std::cout << color::fg_rgb(255, 100, 100) << "Custom red!" << color::_RESET << "\n";
}
```

---

### cursor

ANSI terminal cursor helpers for moving around the screen, clearing lines, hiding/showing the cursor, and updating previously printed lines.

| Function | Description |
|---|---|
| `cursor::up(n)` / `down(n)` / `left(n)` / `right(n)` | Moves the cursor by `n` cells |
| `cursor::home()` | Moves to the terminal home position |
| `cursor::moveTo(row, col)` | Moves to a specific row and column |
| `cursor::clearLine()` | Clears the current line |
| `cursor::clearToEnd()` | Clears from the cursor to the end of the line |
| `cursor::save()` / `restore()` | Saves or restores the cursor position |
| `cursor::hide()` / `show()` | Hides or shows the terminal cursor |
| `cursor::flush()` | Flushes pending cursor output |
| `cursor::updateLine(linesFromBottom, content)` | Rewrites a line above the current cursor anchor |

**Example:**
```cpp
#include "libutils/src/cursor.hpp"
#include <iostream>

int main() {
    std::cout << "Status: waiting\n"
              << "Progress: waiting\n";

    cursor::updateLine(2, "Status: running");
    cursor::updateLine(1, "Progress: 50%");
}
```

---

### File

A comprehensive set of file and directory manipulation tools.

**Example:**
```cpp
#include "libutils/src/File.hpp"
#include <iostream>

int main() {
    std::vector<std::string> content = {"Hello", "World"};
    File::writefile("hello.txt", content);

    for (const auto& line : File::readfile("hello.txt")) {
        std::cout << line << "\n";
    }
}
```

---

### Input

Type-safe terminal input via `std::cin` or `std::getline`. Most methods return `std::optional<T>` so failures are explicit rather than silent.

| Method | Description |
|---|---|
| `Input::read<T>()` | Reads a single value via `cin >>` |
| `Input::read<T>(label)` | Same, but prints a prompt first |
| `Input::readline<T>()` | Reads a full line and parses it into `T` |
| `Input::readline<T>(label)` | Same, but prints a prompt first |
| `Input::readInRange<T>(min, max, label)` | Reads a numeric value and accepts it only if it is inside the inclusive range |
| `Input::readUntilValid<T>(label, errMsg)` | Keeps prompting until parsing succeeds, then returns `T` |
| `Input::readChar(label)` | Reads one raw character with `std::cin.get` |
| `Input::readOneOf<T>({choices}, label)` | Reads a value and accepts it only if it matches one of the allowed choices |
| `Input::readWithDefault<T>(def, label)` | Returns `def` when the user submits an empty or invalid line |
| `Input::readSilent(label)` | Reads a hidden line, useful for passwords |
| `Input::readParsed<T>(parser, label)` | Reads a line and passes it to a custom parser returning `std::optional<T>` |

On failure, `std::nullopt` is returned and `cin` is automatically cleared.

**Example:**
```cpp
#include "libutils/src/Input.hpp"
#include <iostream>
#include <optional>
#include <sstream>

int main() {
    auto age = Input::read<int>("Enter your age: ");
    if (!age) {
        std::cerr << "Invalid input\n";
        return 1;
    }
    std::cout << "Age: " << *age << "\n";

    auto name = Input::readline<std::string>("Enter your name: ");
    if (name) {
        std::cout << "Hello, " << *name << "\n";
    }

    auto rating = Input::readInRange<int>(1, 5, "Rating 1-5: ");
    auto choice = Input::readOneOf<char>({'y', 'n'}, "Continue? ");
    int retries = Input::readWithDefault<int>(3, "Retries [3]: ");
    std::string password = Input::readSilent("Password: ");

    auto even = Input::readParsed<int>([](const std::string& text) -> std::optional<int> {
        std::istringstream ss(text);
        int value;
        if ((ss >> value) && value % 2 == 0) {
            return value;
        }
        return std::nullopt;
    }, "Even number: ");
}
```

---

### LoadingBar

Threaded terminal loading helpers in the `Loadingbar` namespace.

| Class | Description |
|---|---|
| `Loadingbar::Spinner` | Animates a custom sequence of frames beside a message |
| `Loadingbar::StatusLine` | Rewrites a single status line on an interval |

Both classes start their worker thread on construction, support `setMsg(...)`, stop on destruction, and can be stopped manually with `stop()`.

**Example:**
```cpp
#include "libutils/src/LoadingBar.hpp"
#include <chrono>
#include <thread>
#include <vector>

int main() {
    std::vector<std::string> frames = {"|", "/", "-", "\\"};
    Loadingbar::Spinner spinner(frames, 100, "Working");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    spinner.setMsg("Almost done");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    spinner.stop();
}
```

---

### Log

A level-based logger with variadic support and ANSI color output. The `terminate` flag is the **first argument** to `Log::error`.

| Level | Color | Exits? |
|---|---|---|
| `Debug` | Bold Yellow | No |
| `Info` | Green | No |
| `Warn` | Yellow | No |
| `Error` | Red | Optional (`terminate` flag) |

**Example:**
```cpp
#include "libutils/src/Log.hpp"

int main() {
    Log::setLogLevel(Log::LogLevel::Debug);

    Log::debug("Starting up");
    Log::info("Server running on port ", 8080);
    Log::warn("Low memory: ", 42, "MB remaining");
    Log::error(false, "Something went wrong, but we continue");
    Log::error(true, "Fatal error, goodbye"); // calls exit()
}
```

---

### numutils

A namespace of number formatting and conversion helpers. No classes, just functions — include and use.

| Function | Example output |
|---|---|
| `numutils::groupDigits(1234567)` | `"1,234,567"` |
| `numutils::fixed(3.14159, 2)` | `"3.14"` |
| `numutils::zeroPad(42, 5)` | `"00042"` |
| `numutils::human(1532000)` | `"1.53M"` |
| `numutils::toBase(255, 16)` | `"ff"` |
| `numutils::binary(42, true)` | `"0b101010"` |
| `numutils::hex(255, true)` | `"0xFF"` |
| `numutils::bytes(12345678)` | `"11.77 MB"` |
| `numutils::percent(0.853)` | `"85.30%"` |
| `numutils::scientific(123456)` | `"1.23e+05"` |
| `numutils::digits(12345)` | `5` |
| `numutils::ordinal(3)` | `"3rd"` |
| `numutils::time_ms(1534)` | `"1.53s"` |
| `numutils::align(42, 5)` | `"   42"` |

Also provides a `Duration` struct and `duration(seconds)` / `durationToString(d)` helpers.

**Example:**
```cpp
#include "libutils/src/numutils.hpp"
#include <iostream>

int main() {
    std::cout << numutils::groupDigits(9876543) << "\n"; // 9,876,543
    std::cout << numutils::bytes(1048576)       << "\n"; // 1.00 MB
    std::cout << numutils::ordinal(21)          << "\n"; // 21st
    std::cout << numutils::time_ms(90500)       << "\n"; // 1m 30s

    auto d = numutils::duration(3661);
    std::cout << numutils::durationToString(d)  << "\n"; // 1h 1m 1s
}
```

---

### Random

A namespace of random generation utilities backed by `std::mt19937_64`. Auto-seeded from the system clock on first use, or manually seeded via `Random::seed()`.

| Function | Description |
|---|---|
| `Random::getint(min, max)` | Random `uint64_t` in `[min, max]` |
| `Random::getdouble(min, max)` | Random `double` in `[min, max]` |
| `Random::getbool()` | 50/50 coin flip |
| `Random::chance(p)` | Returns `true` with probability `p` (0.0–1.0) |
| `Random::normalDistribution(mean, stddev)` | Sample from a normal distribution |
| `Random::shuffle(vec)` | Shuffles a `std::vector<T>` in-place |
| `Random::shuffle(str)` | Shuffles a `std::string` in-place |
| `Random::getFrom(vec)` | Returns a random element from a vector |
| `Random::getFrom(str)` | Returns a random `char` from a string |
| `Random::generateUUID(hyphen)` | Generates a UUID string, optionally hyphenated |
| `Random::seed(n)` | Manually re-seeds the engine (`0` = re-seed from clock) |

**Example:**
```cpp
#include "libutils/src/Random.hpp"
#include <iostream>

int main() {
    std::cout << Random::getint(1, 100)       << "\n"; // e.g. 42
    std::cout << Random::getdouble(0.0, 1.0)  << "\n"; // e.g. 0.713
    std::cout << Random::getbool()            << "\n"; // 0 or 1
    std::cout << Random::chance(0.1)          << "\n"; // true ~10% of the time
    std::cout << Random::generateUUID(true)   << "\n"; // e.g. a3f1c2d4-...

    std::vector<int> v = {1, 2, 3, 4, 5};
    Random::shuffle(v);

    std::string charset = "abcdef";
    std::cout << Random::getFrom(charset) << "\n"; // random char from "abcdef"
}
```

---

### strutils

A namespace of string padding, trimming, and transformation helpers.

| Function | Description |
|---|---|
| `strutils::repeat(s, n)` | Repeats string `s` n times |
| `strutils::pad_left(s, width)` | Right-aligns `s` in a field of `width` |
| `strutils::pad_right(s, width)` | Left-aligns `s` in a field of `width` |
| `strutils::center(s, width)` | Centers `s` in a field of `width` |
| `strutils::ltrim(s)` | Strips leading whitespace |
| `strutils::rtrim(s)` | Strips trailing whitespace |
| `strutils::trim(s)` | Strips both sides |
| `strutils::trim_chars(s, chars)` | Strips specific characters from both sides |
| `strutils::strip_prefix(s, prefix)` | Removes prefix if present |
| `strutils::strip_suffix(s, suffix)` | Removes suffix if present |

**Example:**
```cpp
#include "libutils/src/strutils.hpp"
#include <iostream>

int main() {
    std::cout << strutils::pad_left("hi", 6)          << "\n"; // "    hi"
    std::cout << strutils::center("hi", 6, "-")       << "\n"; // "--hi--"
    std::cout << strutils::trim("  hello  ")           << "\n"; // "hello"
    std::cout << strutils::strip_prefix("--flag", "--") << "\n"; // "flag"
}
```

---

### Table

Create formatted text tables for terminal output.

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
}
```

---

### TextEditor

A terminal-based text editor component. Supports view-only and edit modes, pagination, line navigation, find, insert, remove, and file metadata display.

**Keybindings (edit mode):**

| Key | Action |
|---|---|
| `i` | Insert line at cursor |
| `a` | Append line after cursor |
| `r` | Remove current line |
| `l` | Jump to line number |
| `f` | Find text (wraps around) |
| `g` / `G` | Jump to start / end |
| `n` / ↓ / Space | Next line |
| `b` / ↑ / Backspace | Previous line |
| PgUp / PgDn | Page navigation |
| `s` | Toggle soft line wrap |
| `d` | Show file metadata |
| `?` | Show help |
| `q` | Quit |

**Example:**
```cpp
#include "libutils/src/TextEditor.hpp"

int main() {
    TextEditor editor("notes.txt");
    editor.openEditor(); // full edit mode by default

    // or open in read-only:
    editor.setMode(TextEditor::m_Mode::MODE_VIEW_ONLY);
    editor.openEditor();
}
```

---

### Timer

High-precision pausable timer (`Timer`) and a scope-lifetime timer (`ScopedTimer`).

**`Timer`** starts automatically on construction.

| Method | Description |
|---|---|
| `elapsed()` | Returns elapsed seconds as `double` |
| `pause()` | Pauses the timer |
| `resume()` | Resumes after pause |
| `reset()` | Resets to 0, stops |
| `restart()` | Resets to 0, keeps running |

**`ScopedTimer`** fires a callback with the elapsed time when it goes out of scope.

**Example:**
```cpp
#include "libutils/src/Timer.hpp"
#include <iostream>

int main() {
    Timer t;
    // ... do work ...
    std::cout << "Elapsed: " << t.elapsed() << "s\n";

    t.pause();
    // ... not counted ...
    t.resume();

    // Scoped:
    {
        ScopedTimer st("my block", [](double s) {
            std::cout << "Block took: " << s << "s\n";
        });
        // ... work ...
    } // callback fires here
}
```

---

### Tokenizer

Tools for splitting strings into tokens.

**Example:**
```cpp
#include "libutils/src/Tokenizer.hpp"
#include <iostream>

int main() {
    auto tokens = Tokenizer::split("one,two,three", ',');
    for (const auto& t : tokens) {
        std::cout << t << "\n";
    }
}
```

---

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue.

## License

This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.
