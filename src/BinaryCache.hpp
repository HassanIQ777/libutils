/* Part of https://github.com/HassanIQ777/libutils
Made on: 	2025-Sep-29
Last update: 2025-Nov-06 */

#ifndef BINARYCACHE_HPP
#define BINARYCACHE_HPP 

#include <fstream>
#include <string>
#include <vector>
#include <type_traits>

class BinaryCache
{
  public:
	// --- Save a vector of trivially copyable types ---
	template <typename T>
	static void save(const std::string &filename, const std::vector<T> &data)
	{
		static_assert(std::is_trivially_copyable<T>::value,
					  "BinaryCache::save only works with trivially copyable types!");

		std::ofstream out(filename, std::ios::binary);
		if (!out)
			throw std::runtime_error("Failed to open file for writing: " + filename);

		size_t count = data.size();
		out.write(reinterpret_cast<const char *>(&count), sizeof(count));
		out.write(reinterpret_cast<const char *>(data.data()), count * sizeof(T));
	}

	// --- Load a vector of trivially copyable types ---
	template <typename T>
	static void load(const std::string &filename, std::vector<T> &data)
	{
		static_assert(std::is_trivially_copyable<T>::value,
					  "BinaryCache::load only works with trivially copyable types!");

		std::ifstream in(filename, std::ios::binary);
		if (!in)
			throw std::runtime_error("Failed to open file for reading: " + filename);

		size_t count;
		in.read(reinterpret_cast<char *>(&count), sizeof(count));

		data.resize(count);
		in.read(reinterpret_cast<char *>(data.data()), count * sizeof(T));
	}

	// --- Save vector of strings ---
	static void save(const std::string &filename, const std::vector<std::string> &vec)
	{
		std::ofstream out(filename, std::ios::binary);
		if (!out)
			throw std::runtime_error("Failed to open file for writing: " + filename);

		size_t count = vec.size();
		out.write(reinterpret_cast<const char *>(&count), sizeof(count));

		for (const auto &s : vec)
		{
			size_t len = s.size();
			out.write(reinterpret_cast<const char *>(&len), sizeof(len));
			out.write(s.data(), static_cast<long>(len));
		}
	}

	// --- Load vector of strings ---
	static void load(const std::string &filename, std::vector<std::string> &vec)
	{
		std::ifstream in(filename, std::ios::binary);
		if (!in)
			throw std::runtime_error("Failed to open file for reading: " + filename);

		size_t count;
		in.read(reinterpret_cast<char *>(&count), sizeof(count));

		vec.resize(count);
		for (size_t i = 0; i < count; i++)
		{
			size_t len;
			in.read(reinterpret_cast<char *>(&len), sizeof(len));
			vec[i].resize(len);
			in.read(&vec[i][0], static_cast<long>(len));
		}
	}
};

#endif // binarycache.hpp