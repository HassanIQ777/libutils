/* Part of https://github.com/HassanIQ777/libutils
Made on    : 2024 Nov 17
Last update: 2025 Nov 06 */

#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <ctime>
#include <random>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <termios.h>
#include <type_traits> // std::common_type

namespace funcs
{
//########################################################################################################################################
// FUNCTION DEFINITIONS

template <typename T>
void printTimed(T text, int cd = 0, int end_cd = 0); // prints "text" character by character with "cd" cooldown between them, end_cd to pause at the end

template <typename T>
void printCentered(T text, int cd = 0, int end_cd = 0); // prints "text" in the middle of the screen

template <typename T>
void printRight(T text, int cd = 0, int end_cd = 0); // unfortunately this has to be in a new line

void printLeftMiddleRight(const std::string &left, const std::string &middle = "", const std::string &right = "");

template <typename T>
void print(const T &value); //prints anything (literally)

template <typename T, typename... Args>
void print(const T &value, const Args &... args); // same but for infinite many arguments

template <typename T>
std::string str(const T &n); // converts anything (literally) to a string

std::string lowercase(std::string text);				 // same as below
std::string uppercase(std::string text);				 // returns an uppercase version of the provided text
void removeChar(std::string &text, char char_to_remove); // removes all instances of char_to_remove from input
void replaceChar(std::string &text, char old_char, char new_char);

size_t getTerminalWidth();	   // balls
std::string getPlatform(void); // returns a string of the platform the function runs on
void clearTerminal();
//template <typename T = std::string>
//T readInput(const std::string prompt);
std::string currentTime(); // returns a string of current date
void msleep(size_t milliseconds);
std::string getKeyPress(); // returns a string of last key press (multiple characters supported!)

inline bool hasSequence(const std::string &text, const std::string &sequence); // returns true if "sequence" was found in "text"
inline std::string m_hash(const std::string text, const uintmax_t length = 32);

bool isNumber(const std::string &s);
std::vector<std::string> split(const std::string &text, char delimiter);

//########################################################
// PRINTING FUNCTIONS
template <typename T>
void printTimed(T text, int cd, int end_cd)
{
	std::string string_text = funcs::str(text);
	if (cd == 0)
	{
		std::cout << string_text;
		return;
	}
	for (char ch : string_text)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(cd));
		std::cout << ch;
		std::cout.flush();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(end_cd));
	std::cout.flush();
}

template <typename T>
void printCentered(T text, int cd, int end_cd)
{
	std::string string_text = str(text);
	size_t WIDTH = getTerminalWidth();
	if (string_text.length() > WIDTH)
	{
		string_text = string_text.substr(0, WIDTH - 1); // truncate if it exceeds terminal length (this is so lazy)
	}
	size_t padding = (WIDTH - string_text.size()) / 2;
	std::cout << std::string(padding, ' '); // padding spaces
	printTimed(string_text, cd, end_cd);
	//std::cout << std::string(padding, ' '); // end padding (currently doesn't work)
}

template <typename T>
void printRight(T text, int cd, int end_cd)
{
	std::string string_text = str(text);
	size_t WIDTH = getTerminalWidth();
	if (string_text.length() > WIDTH)
	{
		string_text = string_text.substr(0, WIDTH - 1); // truncate if it exceeds terminal length (this is so lazy)
	}
	size_t padding = WIDTH - string_text.size();
	std::cout << std::string(padding, ' '); // padding spaces
	printTimed(string_text, cd, end_cd);
	//std::cout << std::string(padding, ' '); // end padding (currently doesn't work)
}
void printLeftMiddleRight(const std::string &left, const std::string &middle, const std::string &right)
{
	size_t WIDTH = funcs::getTerminalWidth();
	size_t max_left = WIDTH / 3;
	size_t max_middle = WIDTH / 3;
	size_t max_right = WIDTH - (max_left + max_middle); // remainder

	std::string l = left.substr(0, max_left);
	std::string m = middle.substr(0, max_middle);
	std::string r = right.substr(0, max_right);

	size_t used = l.size() + m.size() + r.size();
	size_t remaining = WIDTH - used;
	size_t padding_left = remaining / 2;
	size_t padding_right = remaining - padding_left;

	std::cout << l << std::string(padding_left, ' ') << m << std::string(padding_right, ' ') << r;
}

// Base case:
template <typename T>
void print(const T &value)
{
	std::cout << value;
}

// recursive case:
template <typename T, typename... Args>
void print(const T &value, const Args &... args)
{
	std::cout << value;
	print(args...);
}

//########################################################
// String Operations
template <typename T>
std::string str(const T &n)
{
	std::ostringstream stm;
	stm << n;
	return stm.str();
}

std::string lowercase(std::string text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
	return text;
}

std::string uppercase(std::string text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::toupper(c); });
	return text;
}

//########################################################
// OTHER FUNCTIONS
size_t getTerminalWidth()
{
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
	{
		perror("ioctl");
		return 80; // default width
	}
	return static_cast<size_t>(w.ws_col);
}

std::string getPlatform()
{
	struct utsname buffer;
	if (uname(&buffer) == 0)
	{
		return std::string(buffer.sysname);
	}
	return "Unknown";
}

void clearTerminal()
{
#if defined(_WIN32)
	system("cls");
#else
	system("clear");
#endif
}

// TODO...someday
/*template <typename T>
T readInput(const std::string prompt) {
    T value;
    while (!value) {
        std::cout << prompt;
        std::getline(std::cin,value);
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input. Try again.\n";
    }
    return value;
}*/

std::string currentTime()
{
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm tm = *std::localtime(&time);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); // always 19 characters long
	return oss.str();
}

void msleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

std::string getKeyPress()
{
	struct termios oldt, newt;
	unsigned char ch;
	std::string sequence;

	// Save old terminal settings
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	// Turn off canonical mode and echo
	newt.c_lflag &= ~(static_cast<unsigned int>(ICANON | ECHO));
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Read the first character
	ch = static_cast<unsigned char>(getchar());
	sequence += ch;

	// Check escape sequence
	if (ch == '\033')
	{
		// read more chars
		sequence += static_cast<unsigned char>(getchar()); // one more
		sequence += static_cast<unsigned char>(getchar()); // one more
	}

	// Restore old terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return sequence;
}

void removeChar(std::string &text, char char_to_remove)
{
	text.erase(std::remove(text.begin(), text.end(), char_to_remove), text.end());
}

void replaceChar(std::string &text, char old_char, char new_char)
{
	std::replace(text.begin(), text.end(), old_char, new_char);
}

inline bool hasSequence(const std::string &text, const std::string &sequence)
{
	return text.find(sequence) != std::string::npos;
}

/*
inline std::string m_hash(const std::string text, const uintmax_t length)
{
	const std::string charset = "0123456789abcdef";
	std::string value = "";
	uint64_t seed = 0;

	for (const auto &char_ : text)
	{
		seed += 7 * int(char_) + 0x9e3779b9;
	}

	//cout << "The seed of '" << text << "' is (" << seed << ")" << endl;

	srand(seed);
	for (int i = 0; i < length; i++)
	{
		value += charset[rand() % charset.length()];
	}

	return value; //cout << "Hash: " << value << endl;
}
*/

bool isNumber(const std::string &s)
{
	try
	{
		long double unused_variable = std::stold(s);
		unused_variable += 0; // skull emoji
	}
	catch (...)
	{
		return false;
	}
	return true;
}

std::vector<std::string> split(const std::string &text, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(text);
	std::string item; // creating a copy but no one will notice ;D

	while (std::getline(ss, item, delimiter))
	{
		if (!item.empty()) // skip empty substrings
			tokens.push_back(item);
	}

	return tokens;
}

template <typename T, typename U, typename V>
constexpr auto clamp(const T &n, const U &lo, const V &hi)
{
	using R = std::common_type_t<T, U, V>;
	R nn = n, l = lo, h = hi;
	if (nn < l)
		return l;
	if (nn > h)
		return h;
	return nn;
}

} // namespace funcs
//########################################################

// These violate the namespace scope so no reason to put them inside
inline void __staticAssert_impl(bool expression, const char *file, int line)
{
	if (!expression)
	{
		std::cerr << file << ":" << line << "\n";
		std::exit(EXIT_FAILURE);
	}
}

inline void __staticAssert_impl(bool expression, const std::string &msg, const char *file, int line)
{
	if (!expression)
	{
		std::cerr << file << ":" << line << " -> " << msg << "\n";
		std::exit(EXIT_FAILURE);
	}
}

// single macro that works for 1 or 2 arguments
#define funcs_staticAssert(...) __funcs_staticAssert_dispatch(__VA_ARGS__, __funcs_staticAssert2, __funcs_staticAssert1)(__VA_ARGS__)

// helpers to choose overload
#define __funcs_staticAssert_dispatch(_1, _2, NAME, ...) NAME
#define __funcs_staticAssert1(expr) __staticAssert_impl(expr, __FILE__, __LINE__)
#define __funcs_staticAssert2(expr, msg) __staticAssert_impl(expr, msg, __FILE__, __LINE__)

#endif // funcs.hpp
