/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
	@ use this to log things lmao
Made on:     2025-Jan-5
Last update: 2025-Jun-17
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
		DEBUG,
		INFO,
		WARN,
		ERROR
	};
	
	static LogLevel s_current_log_level;

	static void m_debug(const std::string &message);

	static void m_info(const std::string &message);

	static void m_warn(const std::string &message);

	static void m_error(const std::string &message, bool terminate = true);
};

void Log::m_debug(const std::string &message)
{
	std::cout << "\x1b[1m" << "\x1b[33m" /* Bold Yellow */
			  << "(DEBUG):"
			  << "\x1b[0m " << message << "\n";
}

void Log::m_info(const std::string &message)
{
	std::cout << "\x1b[42m" /* Green */
			  << "(INFO):"
			  << "\x1b[0m " << message << "\n";
}

void Log::m_warn(const std::string &message)
{
	std::cout << "\x1b[33m" /* Yellow */
			  << "(WARNING):"
			  << "\x1b[0m " << message << "\n";
}

void Log::m_error(const std::string &message, bool terminate)
{
	std::cout << "\x1b[31m" /* Red */
			  << "(ERROR):"
			  << "\x1b[0m " << message << "\n";
	if (terminate)
	{
		exit(-1);
	}
}

#endif // log.hpp