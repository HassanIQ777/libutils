/* Part of https://github.com/HassanIQ777/libutils
Made on: 	2025-Dec-07
Last update: 2025-Dec-07 */

#ifndef STRUTILS_HPP
#define STRUTILS_HPP

#include <string>
#include <algorithm>
#include <cctype>

namespace strutils
{
// Repeat a string N times
inline std::string repeat(const std::string &s, size_t n)
{
	std::string out;
	out.reserve(s.size() * n);

	while (n--)
		out += s;

	return out;
}

// Pad left: [........hello]
inline std::string pad_left(const std::string &s, size_t width, const std::string &pad = " ")
{
	if (s.size() >= width)
		return s;

	size_t need = width - s.size();
	return repeat(pad, need / pad.size()) + s;
}

// Pad right: [hello........]
inline std::string pad_right(const std::string &s, size_t width, const std::string &pad = " ")
{
	if (s.size() >= width)
		return s;

	size_t need = width - s.size();
	return s + repeat(pad, need / pad.size());
}

// Center: [.......hello.......]
inline std::string center(const std::string &s, size_t width, const std::string &pad = " ")
{
	if (s.size() >= width)
		return s;

	size_t total = width - s.size();
	size_t left = total / 2;
	size_t right = total - left;

	return repeat(pad, left / pad.size()) + s + repeat(pad, right / pad.size());
}

// Trim left (whitespace)
inline std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
									[](unsigned char c) { return !std::isspace(c); }));
	return s;
}

// Trim right (whitespace)
inline std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
						 [](unsigned char c) { return !std::isspace(c); })
				.base(),
			s.end());
	return s;
}

// Trim both sides (whitespace)
inline std::string trim(std::string s)
{
	return ltrim(rtrim(s));
}

// Trim specific characters (both sides)
inline std::string trim_chars(const std::string &s, const std::string &chars)
{
	size_t start = s.find_first_not_of(chars);
	if (start == std::string::npos)
		return "";

	size_t end = s.find_last_not_of(chars);
	return s.substr(start, end - start + 1);
}

// Strip prefix if exists
inline std::string strip_prefix(const std::string &s, const std::string &prefix)
{
	if (s.rfind(prefix, 0) == 0) // starts with prefix
		return s.substr(prefix.size());

	return s;
}

// Strip suffix if exists
inline std::string strip_suffix(const std::string &s, const std::string &suffix)
{
	if (s.size() >= suffix.size() &&
		s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0)
	{
		return s.substr(0, s.size() - suffix.size());
	}

	return s;
}

} // namespace strutils

#endif // strutils.hpp