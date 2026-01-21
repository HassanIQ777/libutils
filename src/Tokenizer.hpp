/* Part of https://github.com/HassanIQ777/libutils
Made on    : 2024 Nov 22
Last update: 2025 Sep 20 */

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
		p_tokens.clear();
		p_tokens_lower.clear();

		std::stringstream ss(set_tokens_string); // init with input string
		std::string token;

		while (ss >> token) // extract tokens separated by spaces
		{
			p_tokens.push_back(token); // add original token
			std::string lower_token = __tokenizer_functions_namespace__::lowercase(token);
			p_tokens_lower.push_back(lower_token); // add lowercase token
		}
	}

  public:
	void m_reset(std::string new_tokens_string)
	{
		p_tokens_string = new_tokens_string;
		updateVectors(new_tokens_string);
	}

	bool m_match(std::string to_match, bool case_sensitive = false)
	{
		if (!case_sensitive)
		{
			std::string to_match_lower = __tokenizer_functions_namespace__::lowercase(to_match);
			for (const auto &token_lower : p_tokens_lower)
			{
				if (__tokenizer_functions_namespace__::has_sequence(token_lower, to_match_lower))
					return true;
			}
		}
		else // else if left for readability
		{
			for (const auto &token : p_tokens)
			{
				if (__tokenizer_functions_namespace__::has_sequence(token, to_match))
					return true;
			}
		}
		return false;
	}

	bool m_matchExact(std::string to_match, bool case_sensitive = false)
	{
		if (!case_sensitive)
		{
			std::string to_match_lower = __tokenizer_functions_namespace__::lowercase(to_match);
			for (const auto &token_lower : p_tokens_lower)
			{
				if (token_lower == to_match_lower)
					return true;
			}
		}
		else // else if left for readability
		{
			for (const auto &token : p_tokens)
			{
				if (token == to_match)
					return true;
			}
		}
		return false;
	}

	const std::vector<std::string> &getTokens() const { return p_tokens; }
	const std::string &getTokensString() const { return p_tokens_string; }

  public:
	Tokenizer(std::string set_tokens_string) : p_tokens_string(set_tokens_string)
	{
		updateVectors(set_tokens_string);
	}

	Tokenizer() : p_tokens_string("") {}

	std::string p_tokens_string;
	std::vector<std::string> p_tokens;		 //original
	std::vector<std::string> p_tokens_lower; //non-case sensitive
};

#endif // tokenizer.hpp