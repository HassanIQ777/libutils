/* Part of https://github.com/HassanIQ777/libutils
Made on: 	 2024-Nov-20
Last update: 2025-Sep-20 */

#ifndef CLIPARSER_HPP
#define CLIPARSER_HPP

#include <string>

class CLIParser {
private:
  int m_argc;
  char **m_argv; // i love-hate pointers

public:
  CLIParser(int argc, char *argvec[]) : m_argc(argc), m_argv(argvec) {}
  CLIParser() {}

  // true if a certain flag(argument) exists in argv
  bool hasFlag(const std::string &flag) const;

  // returns the index of an argument in argv (-1 if argument doesn't
  // exist), use it to find arguments
  int getIndex(const std::string &arg) const;

  // returns the argument at index
  std::string getArg(const int &index) const;

  // returns value after flag, for example you can find filename
  // after --file FILENAME like: getValue("--file")
  std::string getValue(const std::string &arg) const;

  int getArgc() const;
  char **getArgv() const;
  void setArgs(int argc, char *argvec[]);
};

inline bool CLIParser::hasFlag(
    const std::string &flag) const { // returns true if argument is found
  for (int i = 0; i < m_argc; i++) {
    if (flag == std::string(m_argv[i]))
      return true;
  }
  return false;
}

inline int CLIParser::getIndex(
    const std::string &arg) const { // returns the index of an argument
  for (int i = 0; i < m_argc; i++) {
    if (std::string(m_argv[i]) == arg)
      return i;
  }
  return -1; // not found
}

inline std::string CLIParser::getArg(const int &index) const {
  if (index >= m_argc || index < 0)
    return "";
  return m_argv[index];
}

inline std::string CLIParser::getValue(const std::string &arg) const {
  int index = getIndex(arg);
  if (index == -1 ||
      index + 1 >= m_argc) // flag not found or no value after flag
    return "";
  return m_argv[index + 1];
}

inline int CLIParser::getArgc() const { return m_argc; }

inline char **CLIParser::getArgv() const { return m_argv; }

inline void CLIParser::setArgs(int argc, char *argvec[]) {
  m_argc = argc;
  m_argv = argvec;
}

#endif // CLIParser.hpp
