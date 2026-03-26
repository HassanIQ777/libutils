// texteditor.hpp
/* TextEditor by HassanIQ777
created: 2025-Jun-27
updated: 2025-Jul-9
*/

#ifndef TEXTEDITOR
#define TEXTEDITOR

#include "File.hpp"
#include "funcs.hpp"
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <limits>
#include <sys/stat.h>

// start of class
class TextEditor {
public:
  TextEditor(std::string filename) : p_filename(filename) {
    p_file_content = File::readfile(p_filename);
  }

  enum class m_Mode { MODE_VIEW_ONLY, MODE_EDIT };

  void openEditor();
  void setMode(const m_Mode &mode);
  void showFileData();

private:
  // p for private
  m_Mode p_editor_mode = m_Mode::MODE_EDIT;
  std::string p_filename;
  std::vector<std::string> p_file_content;
  std::size_t p_current_line = 0;
  bool p_soft_overwrap = false;

  std::string p_trimStr(std::string text, std::size_t WIDTH);
  std::string p_getModeString();
  std::string p_formatTime(std::time_t timestamp);
  std::size_t p_getTerminalHeight();
  std::size_t p_getPageSize();
  std::size_t p_getPageStart(std::size_t page_size, std::size_t size);
  std::size_t p_getTotalPages(std::size_t page_size, std::size_t size);
  bool p_findText(const std::string &query);
  void p_showHelp();
};

inline void TextEditor::openEditor() {
  bool running = true;

  while (running) {
    funcs::clearTerminal();

    const std::size_t size = p_file_content.size();
    const std::size_t num_digits = std::to_string(size).size();
    const std::size_t WIDTH = funcs::getTerminalWidth();
    const std::size_t page_size = p_getPageSize();
    const std::size_t page_start = p_getPageStart(page_size, size);
    const std::size_t page_end =
        (size < (page_start + page_size)) ? size : (page_start + page_size);
    const std::size_t current_page =
        (size == 0 || page_size == 0) ? 1 : (page_start / page_size) + 1;
    const std::size_t total_pages = p_getTotalPages(page_size, size);

    for (std::size_t i = 0; i < WIDTH; i++) {
      funcs::print("_");
    }
    funcs::print("\n");

    for (std::size_t i = page_start; i < page_end; i++) {
      std::string line;
      if (!p_soft_overwrap) {
        const std::size_t available_width =
            (WIDTH > (num_digits + 2)) ? (WIDTH - 2 - num_digits) : 0;
        line = p_trimStr(p_file_content[i], available_width);
      } else {
        line = p_file_content[i];
      }
      if (i == p_current_line) {
        std::cout << "\x1b[1m"
                  << "\x1b[33m" << std::setw(static_cast<int>(num_digits))
                  << std::right << i + 1 << "│ "
                  << "\x1b[0m" << line << "\n";
      } else {
        std::cout << "\x1b[38;2;110;110;110m"
                  << std::setw(static_cast<int>(num_digits)) << std::right
                  << i + 1 << "│ "
                  << "\x1b[0m" << line << "\n";
      }
    }

    for (std::size_t i = 0; i < WIDTH; i++) {
      funcs::print("─");
    }
    funcs::print("\n");
    std::string left =
        File::getFileName(p_filename) + " [" + p_getModeString() + "]";
    std::string middle =
        "Ln " + std::to_string((size == 0) ? 0 : (p_current_line + 1)) + "/" +
        std::to_string(size) + " Pg " + std::to_string(current_page) + "/" +
        std::to_string(total_pages);
    std::string right = funcs::currentTime();
    funcs::printLeftMiddleRight(p_trimStr(left, WIDTH / 3),
                                p_trimStr(middle, WIDTH / 3),
                                p_trimStr(right, WIDTH / 3));
    funcs::print("\n");
    funcs::print(
        "'?' help | f find | g/G start/end | PgUp/PgDn pages | d data\n");

    std::string input;
    input = funcs::getKeyPress();

    std::string line_number_str;

    if (input == "i" && p_editor_mode > m_Mode::MODE_VIEW_ONLY) {
      funcs::print("Insert: ");
      std::string line;
      std::getline(std::cin, line);
      File::writeline(p_filename, line, p_current_line);
    } else if (input == "r" && p_editor_mode > m_Mode::MODE_VIEW_ONLY) {
      if (size == 0) {
        continue;
      }
      std::string removed_line = p_file_content[p_current_line];
      if (File::removeline(p_filename, p_current_line)) {
        funcs::print("removed line(", p_current_line + 1, ") '", removed_line,
                     "'\n");
      } else {
        funcs::print("Failed to remove line(", p_current_line + 1, ")\n");
      }

      if (p_current_line > 0) {
        p_current_line--;
      }
      if (removed_line != "") {
        funcs::getKeyPress();
      }
    } else if (input == "?") {
      p_showHelp();
    } else if (input == "d") {
      showFileData();
    } else if (input == "f") {
      funcs::print("Find: ");
      std::string query;
      std::getline(std::cin, query);
      p_findText(query);
    } else if (input == "l" && p_editor_mode > m_Mode::MODE_VIEW_ONLY) {
      std::cout << "Line number: ";
      std::cin >> line_number_str;
      std::cin.ignore();
      std::cin.clear();
      if (!funcs::isNumber(line_number_str)) {
        continue;
      }
      const std::size_t temp =
          static_cast<std::size_t>(std::stoull(line_number_str));

      if (temp > size) {
        p_current_line = (size > 0) ? size - 1 : 0;
      } else if (temp < 1) {
        p_current_line = 0;
      } else {
        p_current_line = temp - 1;
      }
    } else if (input == "g") {
      p_current_line = 0;
    } else if (input == "G") {
      p_current_line = (size > 0) ? size - 1 : 0;
    } else if (input == "\033[5") {
      if (p_current_line > 0) {
        p_current_line =
            (p_current_line > page_size) ? (p_current_line - page_size) : 0;
      }
    } else if (input == "\033[6") {
      if (size > 0) {
        p_current_line = ((p_current_line + page_size) < size)
                             ? (p_current_line + page_size)
                             : (size - 1);
      }
    } else if (input == " " || input == "n" || input == "\033[B") {
      if (size > 0 && p_current_line < size - 1) {
        p_current_line++;
      }
    } else if ((!input.empty() && input[0] == char(127)) || input == "b" ||
               input == "\033[A") {
      if (p_current_line > 0) {
        p_current_line--;
      }
    } else if (((!input.empty() && input[0] == char(10)) || input == "a") &&
               p_editor_mode > m_Mode::MODE_VIEW_ONLY) {
      const std::size_t insert_at =
          (size == 0) ? 0
                      : ((p_current_line < size) ? p_current_line + 1 : size);
      p_current_line = insert_at;
      const auto insert_index = static_cast<std::ptrdiff_t>(p_current_line);
      p_file_content.insert(p_file_content.begin() + insert_index, "");
      File::writefile(p_filename, p_file_content);
    } else if (input == "s") {
      p_soft_overwrap = !p_soft_overwrap;
    } else if (input == "q") {
      running = false;
    } else {
      continue;
    }

    p_file_content = File::readfile(p_filename);
  }
}

inline std::string TextEditor::p_trimStr(std::string text, std::size_t WIDTH) {
  if (WIDTH <= 2) {
    return "";
  }
  if (text.length() <= WIDTH) {
    return text;
  }
  return text.substr(0, WIDTH - 2) + "··";
}

inline void TextEditor::p_showHelp() {
  funcs::print("i(nsert line)\n");
  funcs::print("r(emove line)\n");
  funcs::print("l(ine number) = (", p_current_line + 1, ")\n");
  funcs::print("n(ext line) / Down Arrow / Space\n");
  funcs::print("b(ack a line) / Up Arrow / Backspace\n");
  funcs::print("PgUp / PgDn = page skip\n");
  funcs::print("f(ind text)\n");
  funcs::print("g(oto beginning)\n");
  funcs::print("G(oto end)\n");
  funcs::print("a(dd line)\n");
  funcs::print("d(ata about file)\n");
  funcs::print("s(oft line overwrapping) = ", std::boolalpha, p_soft_overwrap,
               "\n");
  funcs::print("q(uit)\n");
  funcs::getKeyPress();
}

inline void TextEditor::showFileData() {
  funcs::clearTerminal();

  funcs::print("File: ", p_filename, "\n");
  funcs::print("Size: ", File::getfilesize(p_filename), " Bytes\n");
  funcs::print("Lines: ", p_file_content.size(), "\n");

  std::string creation_time = "Unavailable";
#ifdef __linux__
  struct stat file_stat{};
  if (stat(p_filename.c_str(), &file_stat) == 0) {
#if defined(st_birthtime)
    creation_time = p_formatTime(file_stat.st_birthtime);
#else
    creation_time = p_formatTime(file_stat.st_ctime);
#endif
  }
#endif
  funcs::print("Creation: ", creation_time, "\n");
  funcs::print("Last Modification: ", File::lastmodification_str(p_filename),
               "\n");
  funcs::print("\nPress any key to continue...");
  funcs::getKeyPress();
}

inline void TextEditor::setMode(const m_Mode &mode) { p_editor_mode = mode; }

inline std::string TextEditor::p_getModeString() {
  std::string mode;
  switch (p_editor_mode) {
  case m_Mode::MODE_VIEW_ONLY:
    mode = "View Only Mode";
    break;

  case m_Mode::MODE_EDIT:
    mode = "Edit Mode";
    break;

  default:
    mode = "???"; // how??
    break;
  }
  return mode;
}

inline std::string TextEditor::p_formatTime(std::time_t timestamp) {
  if (timestamp <= 0) {
    return "Unavailable";
  }

  const std::tm *tm = std::localtime(&timestamp);
  if (tm == nullptr) {
    return "Unavailable";
  }

  std::ostringstream output;
  output << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
  return output.str();
}

inline std::size_t TextEditor::p_getTerminalHeight() {
#ifdef _WIN32
  return 24;
#else
  struct winsize w{};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1 || w.ws_row == 0) {
    return 24;
  }
  return static_cast<std::size_t>(w.ws_row);
#endif
}

inline std::size_t TextEditor::p_getPageSize() {
  const std::size_t terminal_height = p_getTerminalHeight();
  return (terminal_height > 5) ? (terminal_height - 5) : 1;
}

inline std::size_t TextEditor::p_getPageStart(std::size_t page_size,
                                              std::size_t size) {
  if (size == 0 || page_size == 0) {
    return 0;
  }

  return (p_current_line / page_size) * page_size;
}

inline std::size_t TextEditor::p_getTotalPages(std::size_t page_size,
                                               std::size_t size) {
  if (size == 0 || page_size == 0) {
    return 1;
  }

  return (size + page_size - 1) / page_size;
}

inline bool TextEditor::p_findText(const std::string &query) {
  if (query.empty()) {
    return false;
  }

  const std::size_t size = p_file_content.size();
  if (size == 0) {
    return false;
  }

  for (std::size_t i = p_current_line; i < size; i++) {
    if (p_file_content[i].find(query) != std::string::npos) {
      p_current_line = i;
      return true;
    }
  }

  for (std::size_t i = 0; i < p_current_line; i++) {
    if (p_file_content[i].find(query) != std::string::npos) {
      p_current_line = i;
      return true;
    }
  }

  return false;
}
// end of class

#endif
