/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
	@ use this to log things lmao
Made on:     2025-Jan-5
Last update: 2025-Sep-13
*/

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>

class Log
{
  public:
	enum class LogLevel
	{
		log_debug, // weird prefix and lowercase to prevent name clashes with macros from Linux libraries
		log_info,
		log_warn,
		log_error
	};

	static void m_debug(const std::string &message);
	static void m_info(const std::string &message);
	static void m_warn(const std::string &message);
	static void m_error(const std::string &message, bool terminate = true);
	static void m_setLogLevel(const LogLevel &level);

  private:
	static bool p_shouldLog(const LogLevel &log_level);

	static LogLevel p_current_log_level;
};

void Log::m_debug(const std::string &message)
{
	if (!p_shouldLog(LogLevel::log_debug))
	{
		return;
	}

	std::cout << "\x1b[1m"
			  << "\x1b[33m" /* Bold Yellow */
			  << "(DEBUG):"
			  << "\x1b[0m " << message << "\n";
}

void Log::m_info(const std::string &message)
{
	if (!p_shouldLog(LogLevel::log_info))
	{
		return;
	}

	std::cout << "\x1b[42m" /* Green */
			  << "(INFO):"
			  << "\x1b[0m " << message << "\n";
}

void Log::m_warn(const std::string &message)
{
	if (!p_shouldLog(LogLevel::log_warn))
	{
		return;
	}

	std::cout << "\x1b[33m" /* Yellow */
			  << "(WARNING):"
			  << "\x1b[0m " << message << "\n";
}

void Log::m_error(const std::string &message, bool terminate)
{
	if (!p_shouldLog(LogLevel::log_error))
	{
		return;
	}

	std::cout << "\x1b[31m" /* Red */
			  << "(ERROR):"
			  << "\x1b[0m " << message << "\n";
	if (terminate)
	{
		exit(-1);
	}
}

Log::LogLevel Log::p_current_log_level = Log::LogLevel::log_warn;

// I'm sorry for this abomination :3
inline bool operator>=(Log::LogLevel lhs, Log::LogLevel rhs)
{
	return static_cast<int>(lhs) >= static_cast<int>(rhs);
}

void Log::m_setLogLevel(const LogLevel &level)
{
	p_current_log_level = level;
}

bool Log::p_shouldLog(const LogLevel &log_level)
{
	// hierarchy: error > warning > info > debug
	return (log_level >= p_current_log_level);
}

#endif // log.hpp