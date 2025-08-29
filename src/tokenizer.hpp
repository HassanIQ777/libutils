/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
	@ tokenizer.hpp provides the Tokenizer class which allows you to relate tokens to objects then do whatever you want
Made on 2024 Nov 22th
*/

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>

namespace __tokenizer_functions_namespace__ // I'm sorry i had to do this
{
std::string lowercase(std::string text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
	return text;
}
bool has_sequence(const std::string &text, const std::string &sequence)
{
	return text.find(sequence) != std::string::npos;
}
} // namespace __tokenizer_functions_namespace__
class Tokenizer
{
  private:
	void updateVectors(std::string set_tokens_string)
	{
		tokens.clear();
		tokens_lower.clear();

		std::stringstream ss(set_tokens_string); // init with input string
		std::string token;

		while (ss >> token) // extract tokens separated by spaces
		{
			tokens.push_back(token); // add original token
			std::string lower_token = __tokenizer_functions_namespace__::lowercase(token);
			tokens_lower.push_back(lower_token); // add lowercase token
		}
	}

  public:
	void reset(std::string new_tokens_string)
	{
		tokens_string = new_tokens_string;
		updateVectors(new_tokens_string);
	}

	bool match(std::string to_match, bool case_sensitive = false)
	{
		if (!case_sensitive)
		{
			std::string to_match_lower = __tokenizer_functions_namespace__::lowercase(to_match);
			for (const auto &token_lower : tokens_lower)
			{
				if (__tokenizer_functions_namespace__::has_sequence(token_lower, to_match_lower))
					return true;
			}
		}
		else
		{
			for (const auto &token : tokens)
			{
				if (__tokenizer_functions_namespace__::has_sequence(token, to_match))
					return true;
			}
		}
		return false;
	}

  public:
	Tokenizer(std::string set_tokens_string) : tokens_string(set_tokens_string)
	{
		updateVectors(set_tokens_string);
	}
	//Token(){} // this requires more work so I'm not adding it
	std::string tokens_string;
	std::vector<std::string> tokens;	   //original
	std::vector<std::string> tokens_lower; //non-case sensitive
};

#endif // tokenizer.hpp