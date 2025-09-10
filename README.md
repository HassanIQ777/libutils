# libutils
A C++ library that specializes in implementing utility & helper functions for CLI programs and basically anything else, including: file operations, random fucntions, randomization, colors, tokenizing, CLI parsing, benchmarking, logging, timing, and others!


# Headers

## benchmark.hpp
Contains `class CBenchmark` and `struct CBenchmarkResult`.

Allows you to benchmark functions to see how performant they are using `m_run` function.

Also contains `class CycleCounter` that allows you to keep track of CPU cycles. It can be used as follows:
```
CycleCounter cc;
cc.m_start();
foo();
cc.m_stop();
uint64_t cpu_cycles = cc.m_cycles();
```


## cliparser.hpp
Contain `class CLIParser`. Create a CLIParser object like this:

```CLIParser parser(argc, argv);```


## color.hpp
Contains `namespace color` :3


## file.hpp
Contains `class File` which has `static` functions for file operations, directory management, file metadata, and other functions, such as:

```m_readfile, m_writefile, m_checkfile, m_copyfile, m_removefile, m_createdir, m_listfiles, m_listfiles_recursive, m_getfilesize, m_isfile, m_isdirectory, m_lastmodification_t, m_lastmodification_str, m_getFromINI, m_writeToINI, m_sortChronological```

This is by far the most useful header in the library.


## funcs.hpp
Contains `namespace funcs` which has random functions that don't belong elsewhere, such as:

```m_hash, generateUUID, hasSequence, getKeyPress, clearTerminal, getTerminalWidth, printTimed, printCentered```


## pager.hpp
*Work In Progress...*


## random.hpp
Contains `class Random` which has `static` functions for randomization, such as:

```m_int, m_float, m_seed, m_chance, m_normalDistribution, m_shuffle, m_getFrom```



## table.hpp
Contains `class Table`. Create a Table object like this:

```Table table;```
### Usage example:
```
table.m_setHeader("Name", "Score", "Grade");
table.m_addRow("Hassan", 99, "S+");
std::cout << table;
std::ofstream f("filename.txt");
f << table; // writes the table contents to the file
```


## timer.hpp
Contains `class Timer`. Create a Timer object like this:

```Timer timer;```

The timer starts instantly as the object is created.

Contains methods such as:

```m_start, m_pause, m_resume, m_elapsed, m_reset, m_restart```

Also contains `class ScopedTimer`. Use it to time or profile functions, create a scoped timer like this:
```
ScopedTimer(const std::string &name, std::function<void(double)> callback);
```
Note that this is still work in progress!



## texteditor.hpp
Contains `class TextEditor`. Create a TextEditor object like this:

```TextEditor file(std::string filename)```

Contains methods such as:
```m_openEditor, m_setMode```

### *NOTE*: This is still Work In Progress!



## tokenizer.hpp
Contains `class Tokenizer`. Create a Tokenizer object like this:

```Tokenizer obj(std::string set_tokens_string)```

Contains methods such as:
```reset, match```