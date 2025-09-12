/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
	@ use this to generate random functions as well as some other functions 
Made on 2025 May 8
*/

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <chrono>

class Random
{
  public:
	static int m_int(int min, int max); // (include,include)
	static float m_float(float min, float max); // (include,include)
	static void m_seed(unsigned int seed); // if seed == 0 then current time will be chosen
	static bool m_chance(double probability); // (include,include)
	static double m_normalDistribution(double mean, double stddev);
	
	template <typename T>
	static void m_shuffle(std::vector<T> &vec); // for vectors only
	static void m_shuffle(std::string &word); // for strings only
	
	template <typename T>
	static T m_getFrom(const std::vector<T> &vec);
	
	static char m_getFrom(const std::string &word);
	
	static std::string m_generateUUID(bool add_hyphen);

  private:
	static std::mt19937 &p_getEngine();
};

/* PUBLIC members */

int Random::m_int(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(p_getEngine());
}

float Random::m_float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(p_getEngine());
}

bool Random::m_chance(double probability)
{
	std::bernoulli_distribution dist(probability); // probability is from 0.0 to 1.0
	return dist(p_getEngine());
}

double Random::m_normalDistribution(double mean, double stddev)
{
	std::normal_distribution<double> dist(mean, stddev);
	return dist(p_getEngine());
}

template <typename T>
void Random::m_shuffle(std::vector<T> &vec)
{
	std::shuffle(vec.begin(), vec.end(), p_getEngine());
}

void Random::m_shuffle(std::string &word)
{
	std::shuffle(word.begin(), word.end(), p_getEngine());
}

template <typename T>
T Random::m_getFrom(const std::vector<T> &vec)
{
	if (vec.empty())
		return {};
	std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
	return vec[dist(p_getEngine())];
}

char Random::m_getFrom(const std::string &word)
{
	if (word.size() < 2)
	{
		return word[0];
	}
	return word[Random::m_int(0, word.size() - 1)];
}

/* PRIVATE members */

void Random::m_seed(unsigned int seed = 0)
{
	if (seed <= 0)
	{
		p_getEngine().seed(std::chrono::steady_clock::now().time_since_epoch().count());
		return;
	}
	p_getEngine().seed(seed);
}

std::string Random::m_generateUUID(bool add_hyphen)
{
	const std::string charset = "0123456789abcdef";
	std::string uuid;
	constexpr int group_sizes[5] = {8, 4, 4, 4, 12};
	
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < group_sizes[i]; ++j)
		{
			uuid += m_getFrom(charset);
		}
		if (i != 4 && add_hyphen)
		{
			uuid += "-";
		}
	}
	return uuid;
}

std::mt19937 &Random::p_getEngine()
{
	static std::mt19937 engine(static_cast<unsigned int>(
		std::chrono::steady_clock::now().time_since_epoch().count()));
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