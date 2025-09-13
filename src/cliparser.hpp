/*
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.

	@ use CLIParser to parse command line arguments easily 
Made on: 	2024-Nov-20
Last update: 2025-Aug-30
*/

#ifndef CLIPARSER_HPP
#define CLIPARSER_HPP

#include <string>
#include <stdio.h>

class CLIParser
{
  public:
	CLIParser(int argc, char *argvec[]) : argc(argc), argv(argvec) {}

	// FUNCTION NAMES NEED A FUCKING REVISION
	bool m_hasFlag(const std::string &flag) const;		// true if a certain flag(argument) exists in argv

	int m_getIndex(const std::string &arg) const;		 // returns the index of an argument in argv (-1 if argument doesn't exist), use it to find arguments

	std::string m_getArg(const int &index) const; 		// returns the argument at index

	std::string m_getValue(const std::string &arg) const; // returns value after flag, for example you can find filename after --file FILENAME like: getValue("--file")
	
	int m_getArgc() const;
	
	char** m_getArgv() const;

// NOTE : some function names might need revision
  private:
	int argc;
	char **argv; // i love-hate pointers
};

bool CLIParser::m_hasFlag(const std::string &flag) const
{ // returns true if argument is found
	for (unsigned int i = 0; i < argc; i++)
	{
		if (flag == argv[i])
			return true;
	}
	return false;
}

int CLIParser::m_getIndex(const std::string &arg) const
{ // returns the index of an argument
	for (unsigned int i = 0; i < argc; i++)
	{
		if (argv[i] == arg)
			return i;
	}
	return -1; //not found
}

std::string CLIParser::m_getArg(const int &index) const
{
	if (index >= argc || index < 0)
		return "";
	return argv[index];
}

std::string CLIParser::m_getValue(const std::string &arg) const
{
	int index = m_getIndex(arg);
	if (index == -1 || index + 1 >= argc) // flag not found or no value after flag
		return "";
	return argv[index + 1];
}

int CLIParser::m_getArgc() const
{
	return argc;
}
	
char** CLIParser::m_getArgv() const
{
	return argv;
}

#endif // CLIParser.hpp
