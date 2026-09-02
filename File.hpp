/* Part of https://github.com/HassanIQ777/libutils
Made on    : 2024-Nov-02
Last update: 2025-Nov-28 */

#ifndef FILE_HPP
#define FILE_HPP

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class File {
public:
  // ########################################################
  //  all bool functions return true on success, otherwise false

  //		FILE OPERATIONS
  static std::vector<std::string> readfile(const std::string &filename,
                                           size_t reserve_value = 0);
  static void printfile(const std::string &filename); // useless func but yeah
  static bool writefile(const std::string &filename,
                        const std::vector<std::string> &content);
  static bool copyfile(const std::string &source,
                       const std::string &destination);
  static bool removefile(const std::string &path);
  static std::uintmax_t removedir(const std::string &path);
  static bool movefile(const std::string &source,
                       const std::string &destination);
  static std::string readline(const std::string &filename, size_t line_index);
  static bool writeline(const std::string &filename,
                        const std::string &new_line, size_t line_index);
  static bool insertline(const std::string &filename,
                         const std::string &new_line, size_t line_index);
  static bool removeline(const std::string &filename, size_t line_index);
  static bool appendline(const std::string &filename,
                         const std::string &new_line);

  //		FILE & DIRECTORY MANAGEMENT
  static bool createfile(const std::string &filename);
  static bool createdir(const std::string &dir);
  static bool createdirs(const std::string &path);
  static std::vector<std::string> listfiles(const std::string &dir);
  static std::vector<std::string>
  listfiles_recursive(const std::string &dir); // same

private:
  static void
  listfiles_recursive_internal(const std::string &dir,
                               const std::vector<std::string> &exception_list,
                               std::vector<std::string> &file_list);

public:
  static std::vector<std::string>
  listfiles_recursive(const std::string &dir,
                      const std::vector<std::string> &exception_list);

  //		FILE METADATA
  static uintmax_t getfilesize(const std::string &filename);
  static bool isfile(const std::string &path);
  static bool isdirectory(const std::string &path);
  static std::time_t lastmodification_t(const std::string &filename);
  static std::string lastmodification_str(const std::string &filename);
  static size_t numlines(const std::string &filename);

  //		OTHER FUNCTIONS
  static std::string getExtension(const std::string &text);
  static std::string getFileName(const std::string &text);
  static std::string getFromINI(const std::string &filename,
                                const std::string &left,
                                const char delimiter = '=',
                                uint64_t reserve_value = 0);
  static bool writeToINI(const std::string &path, const std::string &left,
                         const std::string &right,
                         const std::string delimiter = "=",
                         uint64_t reserve_value = 0);
  static std::vector<std::string>
  sortChronological(std::vector<std::string> files,
                    const bool &ascending = false);

}; // end of class

// ########################################################
//  File Operations

// returns a vector containing the content of each line in the file |
// reserve_value is how many lines are in the file, not necessary but useful for
// optimization
inline std::vector<std::string> File::readfile(const std::string &filename,
                                               size_t reserve_value) {
  std::vector<std::string> content;
  content.reserve(reserve_value);

  std::ifstream file(filename);
  if (!file.is_open()) {
    return content;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Remove trailing \r (Carriage Return) if it exists
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }

    // Optional: Also remove trailing spaces/tabs that might sneak in
    line.erase(line.find_last_not_of(" \t") + 1, std::string::npos);

    if (!line.empty()) {
      content.push_back(std::move(line));
    }
  }

  file.close();
  return content;
}

// // (completely) replaces old file content with new content
inline bool File::writefile(const std::string &filename,
                            const std::vector<std::string> &content) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  const size_t size = content.size();
  for (size_t i = 0; i < size; i++) {
    if (i != size - 1)
      file << content[i] << '\n';
    else
      file << content[i]; // last line has no newline after it
  }

  file.close();
  return true;
}

// like cp
inline bool File::copyfile(const std::string &source,
                           const std::string &destination) {
  try {
    fs::copy(source, destination, fs::copy_options::overwrite_existing);
    return true;
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error copying file: " << e.what() << '\n';
    return false;
  }
}

// like rm. Only deletes files or empty directories
inline bool File::removefile(const std::string &path) {
  try {
    return fs::remove(path);
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error removing file: " << e.what() << '\n';
    return false;
  }
}

// recursively deletes the contents of a directory and the directory itself, or
// deletes a file entirely. It does not follow symlinks; it removes the symlink
// itself rather than its target. returns a std::uintmax_t count of the number
// of files and directories deleted, returning 0 if the path did not exist.
inline std::uintmax_t File::removedir(const std::string &path) {
  try {
    return fs::remove_all(path);
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error removing file: " << e.what() << '\n';
    return false;
  }
}

// like mv
inline bool File::movefile(const std::string &source,
                           const std::string &destination) {
  try {
    fs::rename(source, destination);
    return true;
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error moving file: " << e.what() << '\n';
    return false;
  }
}

// returns the content of the line of "filename" at line_index
inline std::string File::readline(const std::string &filename,
                                  size_t line_index) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return "";
  }

  std::string line;
  for (size_t i = 0; i <= line_index; ++i) {
    if (!std::getline(file, line)) {
      return ""; // Reached EOF before desired line
    }
  }

  return line;
}

// replaces a single line, DO NOT use with numlines() to append to last line
inline bool File::writeline(const std::string &filename,
                            const std::string &new_line, size_t line_index) {
  std::vector<std::string> content;
  if (fs::exists(filename)) {
    content = File::readfile(filename);
  } else {
    return false;
  }
  if (line_index >= content.size()) {
    content.resize(line_index + 1, ""); // resize vector
  }
  content[line_index] = new_line;
  return File::writefile(filename, content); // write the updated line content
}

inline bool File::insertline(const std::string &filename,
                             const std::string &new_line, size_t line_index) {
  if (!fs::exists(filename)) {
    return false;
  }
  std::vector<std::string> file_content = File::readfile(filename);

  if (line_index >= file_content.size()) {
    file_content.resize(line_index + 1, ""); // resize vector
  }

  file_content.insert(file_content.begin() + static_cast<long>(line_index),
                      new_line);

  return File::writefile(filename,
                         file_content); // write the updated line content
}

// removes the line in "filename" at line_index
inline bool File::removeline(const std::string &filename, size_t line_index) {
  if (!fs::exists(filename)) {
    return false;
  }
  std::vector<std::string> file_content = readfile(filename);
  if (line_index >= file_content.size()) {
    return false;
  }
  file_content.erase(file_content.begin() + static_cast<long>(line_index));
  return writefile(filename, file_content);
}

// appends a line to last line
inline bool File::appendline(const std::string &filename,
                             const std::string &new_line) {
  if (!fs::exists(filename)) {
    return false;
  }

  std::ofstream f(filename, std::ios::app);
  if (!f.is_open()) {
    return false;
  }

  if (fs::file_size(filename) > 0) {
    f << '\n';
  }

  f << new_line;

  return true;
}

// ########################################################
//  File & Directory Management

inline bool File::createfile(const std::string &filename) {
  std::ofstream f(filename);

  if (!fs::exists(filename)) {
    return false;
  }

  return true;
}

// like mkdir
inline bool File::createdir(const std::string &path) {
  try {
    return fs::create_directory(path);
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error creating directory: " << e.what() << '\n';
    return false;
  }
}

// like mkdir -p
inline bool File::createdirs(const std::string &path) {
  try {
    return fs::create_directories(path);
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error creating directory: " << e.what() << '\n';
    return false;
  }
}

// lists files in dir
inline std::vector<std::string> File::listfiles(const std::string &dir) {
  std::vector<std::string> files;
  try {
    for (const auto &entry : fs::directory_iterator(dir)) {
      files.push_back(entry.path().string());
    }
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error listing files: " << e.what() << '\n';
  }
  return files;
}

// lists files in dir but recursive, meaning every single file that traces back
// to mother directory (dir). dirs/files in exception_list won't be included
inline std::vector<std::string>
File::listfiles_recursive(const std::string &dir) {
  std::vector<std::string> files;
  for (const auto &entry : fs::recursive_directory_iterator(dir)) {
    files.push_back(entry.path().string());
  }
  return files;
}

// Pass file_list by reference (&) to avoid massive copying overhead
inline void File::listfiles_recursive_internal(
    const std::string &dir, const std::vector<std::string> &exception_list,
    std::vector<std::string> &file_list) {
  if (!fs::exists(dir) || !fs::is_directory(dir))
    return;

  try {
    for (const auto &entry : fs::directory_iterator(dir)) {
      std::string current_path = entry.path().string();

      // 1. Check exceptions
      if (std::find(exception_list.begin(), exception_list.end(),
                    current_path) != exception_list.end()) {
        continue;
      }

      // 2. Distinguish between Files and Directories
      if (fs::is_regular_file(entry.status())) {
        file_list.push_back(current_path); // Only add actual files
      } else if (fs::is_directory(entry.status())) {
        // Recurse using the same vector reference
        File::listfiles_recursive_internal(current_path, exception_list,
                                           file_list);
      }
    }
  } catch (const fs::filesystem_error &) {
    // Log or ignore permission denied errors
  }
}

// Public wrapper function
inline std::vector<std::string>
File::listfiles_recursive(const std::string &dir,
                          const std::vector<std::string> &exception_list) {
  std::vector<std::string> result;
  listfiles_recursive_internal(dir, exception_list, result);
  return result;
}

// ########################################################
//  File Metadata

// returns file size in Bytes
inline uintmax_t File::getfilesize(const std::string &filename) {
  try {
    return fs::file_size(filename);
  } catch (const fs::filesystem_error &e) {
    // std::cerr << "Error getting file size: " << e.what() << '\n';
    return 0;
  }
}

// true if "path" leads to a file
inline bool File::isfile(const std::string &path) {
  return fs::is_regular_file(path);
}

// true if "path" leads to a directory
inline bool File::isdirectory(const std::string &path) {
  return fs::is_directory(path);
}

// returns last modification time of filename as an integer, for example:
// 1735910400
inline std::time_t File::lastmodification_t(const std::string &filename) {
  auto ftime = fs::last_write_time(filename);
  auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
      ftime - fs::file_time_type::clock::now() +
      std::chrono::system_clock::now());
  return std::chrono::system_clock::to_time_t(sctp);
}

// same but in a readable string format, for example: 2025-01-03 12:00:00
inline std::string File::lastmodification_str(const std::string &filename) {
  std::time_t mod_time = File::lastmodification_t(filename);
  std::tm *local_tm = std::localtime(&mod_time);
  std::ostringstream oss;
  oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

// 2nd useless function
inline size_t File::numlines(const std::string &filename) {
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    return 0;
  }

  constexpr size_t BUFFER_SIZE = 1 << 21; // 2MB buffer
  char buffer[BUFFER_SIZE];
  size_t line_count = 0;
  bool has_any_byte = false;
  bool last_char_newline = false;

  while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) {
    std::streamsize bytes_read = file.gcount();
    if (bytes_read > 0) {
      has_any_byte = true;
      last_char_newline = (buffer[bytes_read - 1] == '\n');
    }
    for (std::streamsize i = 0; i < bytes_read; ++i) {
      if (buffer[i] == '\n') {
        ++line_count;
      }
    }
  }

  // Count the final non-empty line when file does not end with '\n'.
  if (has_any_byte && !last_char_newline) {
    ++line_count;
  }

  return line_count;
}

// ########################################################
//  Other Functions

// returns extension of given text/file in ".XYZ" format.
// NOTE: WITH THE DOT
inline std::string File::getExtension(const std::string &text) {
  return fs::path(text).extension().string();
}

inline std::string File::getFileName(const std::string &text) {
  return fs::path(text).filename().string();
}

// i saw this in Minecraft's options.txt
inline std::string File::getFromINI(const std::string &filename,
                                    const std::string &left,
                                    const char delimiter,
                                    uint64_t reserve_value) {
  /* I recommend using this as a delimiter:
  char delimiter = 0x1F;
  std::string d(1, delimiter); */

  const std::vector<std::string> content =
      File::readfile(filename, reserve_value);
  size_t at;
  std::pair<std::string, std::string> left_right;

  for (const std::string &line : content) {
    at = line.find(delimiter);

    if (at == std::string::npos)
      continue;

    left_right = {line.substr(0, at), line.substr(at + 1, line.length())};
    if (left_right.first == left) {
      return left_right.second;
    }
  }

  return "";
}

inline bool File::writeToINI(const std::string &path, const std::string &left,
                             const std::string &right,
                             const std::string delimiter,
                             uint64_t reserve_value) {
  std::vector<std::string> content = File::readfile(path, reserve_value);
  size_t at = std::string::npos;
  size_t index = 0;
  bool found = false;
  std::pair<std::string, std::string> left_right;

  for (const std::string &line : content) {
    at = line.find(delimiter);

    if (at == std::string::npos)
      continue;

    left_right = {line.substr(0, at), line.substr(at + 1, line.length())};
    if (left_right.first == left) {
      left_right.second = right;
      found = true;
      break;
    }
    index++;
  }

  if (!found) {
    return false;
  }

  return File::writeline(path, left_right.first + delimiter + left_right.second,
                         index);
}

inline std::vector<std::string>
File::sortChronological(std::vector<std::string> files, const bool &ascending) {
  // no need to remove directories!
  /*
  files.erase(std::remove_if(files.begin(),
                                                     files.end(),
                                                     [](std::string &file) {
                                                             return
  !isfile(file);
                                                     }),
                          files.end());
  */

  std::vector<std::pair<std::string, uint64_t>> files_dates;
  for (const auto &f : files) {
    files_dates.push_back({f, lastmodification_t(f)});
  }

  std::sort(files_dates.begin(), files_dates.end(),
            [&ascending](const std::pair<std::string, int> &lhs,
                         const std::pair<std::string, int> &rhs) {
              return ascending ? (lhs.second < rhs.second)
                               : (lhs.second > rhs.second);
            });

  std::vector<std::string> sorted_files;
  for (const auto &[f, d] : files_dates) {
    sorted_files.push_back(f);
  }

  return sorted_files;
}

// ########################################################

#endif // file.hpp
