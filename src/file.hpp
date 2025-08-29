/*
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.

	@ used file.hpp to manage, create, read, write to, check, files and directories and their data and metadata
	@ this was rewritten using <filesystem>
Made on 		  2024-Nov-2
Last update as of 2025-Aug-14
*/

/*
Future additions:
	@ Use log.hpp to log errors but use log levels
	@ More functions maybe? but for what, what do we need?
	@ bruh I'm one person talking to myself
*/

#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <random>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace fs = std::filesystem;

class File
{
  public:
	//########################################################
	// all bool functions return true on success, otherwise false

	//		FILE OPERATIONS
	static std::vector<std::string> m_readfile(const std::string &filename, int reserve_value = 0);	   // returns a vector containing the content of each line in the file | reserve_value is how many lines are in the file, not necessary but useful for optimization
	static void m_printfile(const std::string &filename);											   // useless func but yeah
	static bool m_writefile(const std::string &filename, const std::vector<std::string> &content);	   // (completely) replaces old file content with new content
	static bool m_checkfile(const std::string &filename, bool create_file = false);					   // trash
	static bool m_copyfile(const std::string &source, const std::string &destination);				   // like cp
	static bool m_removefile(const std::string &filename);											   // like rm, i didn't actually test if it can remove dirs tho
	static bool m_movefile(const std::string &source, const std::string &destination);				   // like mv
	static std::string m_readline(const std::string &filename, int line_index);						   // returns the content of the line of "filename" at line_index
	static bool m_writeline(const std::string &filename, const std::string &new_line, int line_index); // replaces a single line, DO NOT use with numlines() to append to last line
	static bool m_insertline(const std::string &filename, const std::string &new_line, int line_index);
	static bool m_removeline(const std::string &filename, int line_index);				// removes the line in "filename" at line_index
	static bool m_appendline(const std::string &filename, const std::string &new_line); // appends a line to last line

	//		DIRECTORY MANAGEMENT
	static bool m_createdir(const std::string &dir);																			   // like mkdir
	static std::vector<std::string> m_listfiles(const std::string &dir);														   // lists files in dir
	static std::vector<std::string> m_listfiles_recursive(const std::string &dir);												   // same but recursive, meaning every single file that traces back to mother directory (dir)
	static std::vector<std::string> m_listfiles_recursive(const std::string &dir, const std::vector<std::string> &exception_list); // dirs/files in exception_list won't be included

	//		FILE METADATA
	static uintmax_t m_getfilesize(const std::string &filename);			// returns file size in Bytes
	static bool m_isfile(const std::string &path);							// true if "path" leads to a file
	static bool m_isdirectory(const std::string &path);						// same but for directories
	static std::time_t m_lastmodification_t(const std::string &filename);	// returns last modification time of filename as an integer, for example: 1735910400
	static std::string m_lastmodification_str(const std::string &filename); // same but in a readable string format, for example: 2025-01-03 12:00:00
	static int m_numlines(const std::string &filename);						// 2nd useless function, but I use to access the last line in a file

	//		OTHER FUNCTIONS
	static std::string m_getExtension(const std::string &text); // returns extension of given text/file in ".XYZ" format
	static std::string m_getFileName(const std::string &text);
	static std::string m_getFromINI(const std::string &filename, const std::string &left, const char delimiter = '=', uint64_t reserve_value = 0); // i saw this in Minecraft Bedrock Edition
	static void m_writeToINI(const std::string &path, const std::string &left, const std::string &right, const std::string delimiter = "=", uint64_t reserve_value = 0);
	static std::vector<std::string> m_sortChronological(std::vector<std::string> files, const bool &ascending = false);

}; // end of class

//########################################################
// File Operations
std::vector<std::string> File::m_readfile(const std::string &filename, int reserve_value)
{
	std::vector<std::string> content;
	content.reserve(reserve_value);

	std::ifstream file(filename);
	if (!file.is_open())
	{
		return content;
	}

	std::string line;
	while (std::getline(file, line))
	{
		content.push_back(std::move(line));
	}

	file.close();
	return content;
}

bool File::m_writefile(const std::string &filename, const std::vector<std::string> &content)
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		return false;
	}

	const size_t size = content.size();
	for (size_t i = 0; i < size; i++)
	{
		if (i != size - 1)
			file << content[i] << '\n';
		else
			file << content[i]; // last line has no newline after it
	}

	file.close();
	return true;
}

bool File::m_checkfile(const std::string &filename, bool create_file)
{
	if (!fs::exists(filename) && create_file)
	{
		std::ofstream(filename).close();
		return false; // file doesn't exist and we just created it
	}
	if (!fs::exists(filename) && !create_file)
	{
		return false; // file doesn't exist and we didn't create it
	}
	return true; // true means: 1:(exists && create) 2:(exists && don't create)
}

bool File::m_copyfile(const std::string &source, const std::string &destination)
{
	try
	{
		fs::copy(source, destination, fs::copy_options::overwrite_existing);
		return true;
	}
	catch (const fs::filesystem_error &e)
	{
		// std::cerr << "Error copying file: " << e.what() << '\n';
		return false;
	}
}

bool File::m_removefile(const std::string &filename)
{
	try
	{
		return fs::remove(filename);
	}
	catch (const fs::filesystem_error &e)
	{
		// std::cerr << "Error removing file: " << e.what() << '\n';
		return false;
	}
}

bool File::m_movefile(const std::string &source, const std::string &destination)
{
	try
	{
		fs::rename(source, destination);
		return true;
	}
	catch (const fs::filesystem_error &e)
	{
		// std::cerr << "Error moving file: " << e.what() << '\n';
		return false;
	}
}

std::string File::m_readline(const std::string &filename, int line_index)
{
	if (line_index < 0)
	{
		return "";
	}

	std::ifstream file(filename);
	if (!file.is_open())
	{
		return "";
	}

	std::string line;
	for (int i = 0; i <= line_index; ++i)
	{
		if (!std::getline(file, line))
		{
			return ""; // Reached EOF before desired line
		}
	}

	return line;
}

bool File::m_writeline(const std::string &filename, const std::string &new_line, int line_index)
{
	if (line_index < 0)
	{
		return false;
	}
	std::vector<std::string> content;
	if (fs::exists(filename))
	{
		content = File::m_readfile(filename);
	}
	else
	{
		return false;
	}
	if (line_index >= static_cast<int>(content.size()))
	{
		content.resize(line_index + 1, ""); // resize vector
	}
	content[line_index] = new_line;
	File::m_writefile(filename, content); // write the updated line content
	return true;
}

bool File::m_insertline(const std::string &filename, const std::string &new_line, int line_index)
{
	if (line_index < 0 || !fs::exists(filename))
	{
		return false;
	}
	std::vector<std::string> file_content = File::m_readfile(filename);

	if (line_index >= static_cast<int>(file_content.size()))
	{
		file_content.resize(line_index + 1, ""); // resize vector
	}

	file_content.insert(file_content.begin() + line_index, new_line);

	File::m_writefile(filename, file_content); // write the updated line content
	return true;
}

bool File::m_removeline(const std::string &filename, int line_index)
{
	if (!fs::exists(filename))
	{
		return false;
	}
	std::vector<std::string> file_content = m_readfile(filename);
	if (line_index < 0 || line_index >= file_content.size())
	{
		return false;
	}
	file_content.erase(file_content.begin() + line_index);
	m_writefile(filename, file_content);
	return true;
}

bool File::m_appendline(const std::string &filename, const std::string &new_line)
{
	if (!fs::exists(filename))
	{
		return false;
	}
	std::vector<std::string> file_content = m_readfile(filename);

	file_content.push_back(new_line);
	m_writefile(filename, file_content);
	return true;
}

//########################################################
// Directory Management

bool File::m_createdir(const std::string &dir)
{
	try
	{
		return fs::create_directory(dir);
	}
	catch (const fs::filesystem_error &e)
	{
		//std::cerr << "Error creating directory: " << e.what() << '\n';
		return false;
	}
}

std::vector<std::string> File::m_listfiles(const std::string &dir)
{
	std::vector<std::string> files;
	try
	{
		for (const auto &entry : fs::directory_iterator(dir))
		{
			files.push_back(entry.path().string());
		}
	}
	catch (const fs::filesystem_error &e)
	{
		//std::cerr << "Error listing files: " << e.what() << '\n';
	}
	return files;
}

std::vector<std::string> File::m_listfiles_recursive(const std::string &dir)
{
	std::vector<std::string> files;
	for (const auto &entry : fs::recursive_directory_iterator(dir))
	{
		files.push_back(entry.path().string());
	}
	return files;
}

std::vector<std::string> File::m_listfiles_recursive(const std::string &dir, const std::vector<std::string> &exception_list)
{
	std::vector<std::string> file_list;

	if (!fs::exists(dir) || !fs::is_directory(dir))
	{
		return file_list;
	}

	try
	{
		for (const auto &entry : fs::directory_iterator(dir))
		{
			std::string current_path = entry.path().string();

			// Check if the current_path is in the exception_list
			// Using std::find for simple linear search. For very large exception lists,
			// consider using std::unordered_set for O(1) average time complexity.
			if (std::find(exception_list.begin(), exception_list.end(), current_path) != exception_list.end())
			{
				// This path is in the exception_list, so skip it (do not add to file_list and do not recurse)
				continue;
			}

			// If not an exception, add the current entry's path to the list
			file_list.push_back(current_path);

			// If the current entry is a directory AND it's not in the exception_list, recurse into it
			if (fs::is_directory(entry.status()))
			{
				// Get the sub-list of files/dirs from the recursive call
				// Pass the exception_list down to the recursive call
				std::vector<std::string> sub_list = File::m_listfiles_recursive(current_path, exception_list);
				// Append the sub-list to the main list
				file_list.insert(file_list.end(), sub_list.begin(), sub_list.end());
			}
		}
	}
	catch (const fs::filesystem_error &ex)
	{
		std::cerr << "Filesystem error: " << ex.what() << " - Path: " << dir << std::endl;
		// Depending on requirements, you might rethrow, return partially, or return empty
	}

	return file_list;
}

//########################################################
// File Metadata

uintmax_t File::m_getfilesize(const std::string &filename)
{
	try
	{
		return fs::file_size(filename);
	}
	catch (const fs::filesystem_error &e)
	{
		//std::cerr << "Error getting file size: " << e.what() << '\n';
		return 0;
	}
}

bool File::m_isfile(const std::string &path)
{
	return fs::is_regular_file(path);
}

bool File::m_isdirectory(const std::string &path)
{
	return fs::is_directory(path);
}

std::time_t File::m_lastmodification_t(const std::string &filename)
{
	auto ftime = fs::last_write_time(filename);
	auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
		ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
	return std::chrono::system_clock::to_time_t(sctp);
}

std::string File::m_lastmodification_str(const std::string &filename)
{
	std::time_t mod_time = File::m_lastmodification_t(filename);
	std::tm *local_tm = std::localtime(&mod_time);
	std::ostringstream oss;
	oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

int File::m_numlines(const std::string &filename)
{
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		return 0;
	}

	constexpr size_t BUFFER_SIZE = 1 << 21; // 2MB buffer
	char buffer[BUFFER_SIZE];
	int line_count = 0;

	while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0)
	{
		std::streamsize bytes_read = file.gcount();
		for (std::streamsize i = 0; i < bytes_read; ++i)
		{
			if (buffer[i] == '\n')
			{
				++line_count;
			}
		}
	}

	return line_count;
}

//########################################################
// Other Functions

std::string File::m_getExtension(const std::string &text)
{
	return fs::path(text).extension().string();
}

std::string File::m_getFileName(const std::string &text)
{
	return fs::path(text).filename().string();
}

std::string File::m_getFromINI(const std::string &filename, const std::string &left, const char delimiter, uint64_t reserve_value)
{
	const std::vector<std::string> content = File::m_readfile(filename, reserve_value);
	int at;
	std::pair<std::string, std::string> left_right;

	for (const std::string &line : content)
	{
		at = line.find(delimiter);
		left_right = {line.substr(0, at), line.substr(at + 1, line.length())};
		if (left_right.first == left)
		{
			return left_right.second;
		}
	}

	return "";
}

void File::m_writeToINI(const std::string &path, const std::string &left, const std::string &right, const std::string delimiter, uint64_t reserve_value)
{
	std::vector<std::string> content = File::m_readfile(path, reserve_value);
	int at;
	int index = 0;
	std::pair<std::string, std::string> left_right;

	for (const std::string &line : content)
	{
		at = line.find(delimiter);
		left_right = {line.substr(0, at), line.substr(at + 1, line.length())};
		if (left_right.first == left)
		{
			left_right.second = right;
			break;
		}
		index++;
	}

	File::m_writeline(path, left_right.first + delimiter + left_right.second, index);
}

std::vector<std::string> File::m_sortChronological(std::vector<std::string> files, const bool &ascending)
{
	// no need to remove directories!
	/*
	files.erase(std::remove_if(files.begin(),
							   files.end(),
							   [](std::string &file) {
								   return !m_isfile(file);
							   }),
				files.end());
	*/

	std::vector<std::pair<std::string, uint64_t>> files_dates;
	for (const auto &f : files)
	{
		files_dates.push_back({f, m_lastmodification_t(f)});
	}

	std::sort(files_dates.begin(), files_dates.end(), [&ascending](const std::pair<std::string, int> &lhs, const std::pair<std::string, int> &rhs) {
		return ascending ? (lhs.second < rhs.second) : (lhs.second > rhs.second);
	});

	std::vector<std::string> sorted_files;
	for (const auto &[f, d] : files_dates)
	{
		sorted_files.push_back(f);
	}

	return sorted_files;
}

//########################################################

#endif // file.hpp