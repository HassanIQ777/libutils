/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
	@ funcs.hpp has a few functions that I oftentimes need
	@ so i made a header file out of them
Made on:     2024 Nov 17th
Last update: 2025 Jun 16th
*/

#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <ctime>
#include <random>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <termios.h>

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
std::string str(const T &n);			 // converts anything (literally) to a string
std::string lowercase(std::string text); // same as below
std::string uppercase(std::string text); // returns an uppercase version of the provided text
std::string removeChar(const std::string& text, char char_to_remove); // removes all instances of char_to_remove from input
std::string replaceChar(std::string& text, char old_char, char new_char);

int getTerminalWidth();		   // balls
std::string getPlatform(void); // returns a string of the platform the function runs on
void clearTerminal();
//template <typename T = std::string>
//T readInput(const std::string prompt);
std::string currentTime(); // returns a string of current date
void msleep(int milliseconds);
int randomInt(int min, int max); // this works like (include,include)
std::string getKeyPress();		 // returns a string of last key press (multiple characters supported!)

inline bool hasSequence(const std::string &text, const std::string &sequence); // returns true if "sequence" was found in "text"
inline std::string generateUUID(bool add_hyphen = true);					   // returns a uuid like "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" where X can be any hex character
inline std::string m_hash(const std::string text, const uintmax_t length = 32);

bool isNumber(std::string &s);
std::pair<std::string, std::string> split(std::string s, char delimiter);
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
	int WIDTH = getTerminalWidth();
	if (string_text.length() > WIDTH)
	{
		string_text = string_text.substr(0, WIDTH - 1); // truncate if it exceeds terminal length (this is so lazy)
	}
	int padding = (WIDTH - string_text.size()) / 2;
	std::cout << std::string(padding, ' '); // padding spaces
	printTimed(string_text, cd, end_cd);
	//std::cout << std::string(padding, ' '); // end padding (currently doesn't work)
}

template <typename T>
void printRight(T text, int cd, int end_cd)
{
	std::string string_text = str(text);
	int WIDTH = getTerminalWidth();
	if (string_text.length() > WIDTH)
	{
		string_text = string_text.substr(0, WIDTH - 1); // truncate if it exceeds terminal length (this is so lazy)
	}
	int padding = WIDTH - string_text.size();
	std::cout << std::string(padding, ' '); // padding spaces
	printTimed(string_text, cd, end_cd);
	//std::cout << std::string(padding, ' '); // end padding (currently doesn't work)
}
void printLeftMiddleRight(const std::string &left, const std::string &middle, const std::string &right)
{
	int WIDTH = funcs::getTerminalWidth();
	int max_left = WIDTH / 3;
	int max_middle = WIDTH / 3;
	int max_right = WIDTH - (max_left + max_middle); // remainder

	std::string l = left.substr(0, max_left);
	std::string m = middle.substr(0, max_middle);
	std::string r = right.substr(0, max_right);

	int used = l.size() + m.size() + r.size();
	int remaining = WIDTH - used;
	int padding_left = remaining / 2;
	int padding_right = remaining - padding_left;

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
int getTerminalWidth()
{
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
	{
		perror("ioctl");
		return 80; // default width
	}
	return w.ws_col;
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

std::string currentTime() {
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
	char ch;
	std::string sequence;

	// Save old terminal settings
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	// Turn off canonical mode and echo
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Read the first character
	ch = getchar();
	sequence += ch;

	// Check escape sequence
	if (ch == '\033')
	{
		// read more chars
		sequence += getchar(); // one more
		sequence += getchar(); // one more
	}

	// Restore old terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return sequence;
}

std::string removeChar(const std::string &text, char char_to_remove)
{
	std::string result = text;
	result.erase(std::remove(result.begin(), result.end(), char_to_remove), result.end());
	return result;
}

std::string replaceChar(std::string& text, char old_char, char new_char) {
    std::replace(text.begin(), text.end(), old_char, new_char);
    return text;
}

inline bool hasSequence(const std::string &text, const std::string &sequence)
{
	return text.find(sequence) != std::string::npos;
}

inline std::string generateUUID(bool add_hyphen)
{
	const std::string charset = "0123456789abcdef";
	std::string uuid;
	const int group_sizes[5] = {8, 4, 4, 4, 12};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, charset.size() - 1);
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < group_sizes[i]; ++j)
		{
			uuid += charset[dis(gen)];
		}
		if (i != 4 && add_hyphen)
		{
			uuid += "-";
		}
	}
	return uuid;
}
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

bool isNumber(std::string &s)
{
	for (const char &c : s)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return true;
}

std::pair<std::string, std::string> split(std::string s, char delimiter)
{
	int at = s.find(delimiter);
	return {s.substr(0, at), s.substr(at + 1, s.length())};
}

} // namespace funcs
//########################################################

#endif // funcs.hpp
