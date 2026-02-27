/* Part of https://github.com/HassanIQ777/libutils
Made on:     2025 May 08
Last update: 2025 Nov 06 */

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <chrono>
#include <limits>
#include <cmath>
#include <algorithm> // for std::shuffle

class Random
{
  public:
	// basic types
	static uint64_t intValue(uint64_t min, uint64_t max); // (include,include)
	static double doubleValue(double min, double max);	   // (include,include)
	static bool boolValue();

	static void seed(uint64_t seed = 0); // if seed == 0 then current time will be chosen

	static bool chance(double probability); // (include,include)
	static double normalDistribution(double mean, double stddev);

	template <typename T>
	static void shuffle(std::vector<T> &vec); // for vectors only
	static void shuffle(std::string &word);	// for strings only

	template <typename T>
	static T getFrom(const std::vector<T> &vec);

	static char getFrom(const std::string &word);

	static std::string generateUUID(bool add_hyphen);

  private:
	static std::mt19937_64 &p_getEngine();
};

/* PUBLIC members */

inline uint64_t Random::intValue(uint64_t min, uint64_t max)
{
	std::uniform_int_distribution<uint64_t> dist(min, max);
	return dist(p_getEngine());
}

inline double Random::doubleValue(double min, double max)
{
	std::uniform_real_distribution<double> dist(
		min, std::nextafter(max, std::numeric_limits<double>::max()));
	return dist(p_getEngine());
}

inline bool Random::boolValue()
{
	std::bernoulli_distribution dist(0.5); // 50/50 chance
	return dist(p_getEngine());
}

inline bool Random::chance(double probability)
{
	std::bernoulli_distribution dist(probability); // probability is from 0.0 to 1.0
	return dist(p_getEngine());
}

inline double Random::normalDistribution(double mean, double stddev)
{
	std::normal_distribution<double> dist(mean, stddev);
	return dist(p_getEngine());
}

template <typename T>
inline void Random::shuffle(std::vector<T> &vec)
{
	std::shuffle(vec.begin(), vec.end(), p_getEngine());
}

inline void Random::shuffle(std::string &word)
{
	std::shuffle(word.begin(), word.end(), p_getEngine());
}

template <typename T>
inline T Random::getFrom(const std::vector<T> &vec)
{
	if (vec.empty())
		return {};
	std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
	return vec[dist(p_getEngine())];
}

inline char Random::getFrom(const std::string &word)
{
	if (word.empty())
		return '\0';
	if (word.size() == 1)
		return word[0];

	return word[Random::intValue(0, word.size() - 1)];
}

/* PRIVATE members */

inline void Random::seed(uint64_t seed)
{
	if (seed == 0)
		seed = static_cast<size_t>(std::chrono::steady_clock::now().time_since_epoch().count());
	p_getEngine().seed(seed);
}

inline std::string Random::generateUUID(bool add_hyphen)
{
	const std::string charset = "0123456789abcdef";
	std::string uuid;
	constexpr int group_sizes[5] = {8, 4, 4, 4, 12};

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < group_sizes[i]; ++j)
		{
			uuid += getFrom(charset);
		}
		if (i != 4 && add_hyphen)
		{
			uuid += "-";
		}
	}
	return uuid;
}

inline std::mt19937_64 &Random::p_getEngine()
{
	static std::mt19937_64 engine(static_cast<size_t>(std::chrono::steady_clock::now().time_since_epoch().count()));
	return engine;
}

/* Might add these functions in the future:

	Vector2 randomDirection2D();
	
	float bias(float min, float max, float power = 2.0f)
	
	bool skewedBool(double skew = 0.8)
	
	T weightedChoice(const std::vector<T>& items, const std::vector<double>& weights)
	
	int randomSign()
*/

#endif // random.hpp
