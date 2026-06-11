/* Part of https://github.com/HassanIQ777/libutils
Made on:     2025 May 08
Last update: 2026 Jun 12 */

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <algorithm> // for std::shuffle
#include <chrono>
#include <cmath>
#include <limits>
#include <random>

namespace Random {

/* PUBLIC members */
inline std::mt19937_64 &p_getEngine() {
  static std::mt19937_64 engine(static_cast<size_t>(
      std::chrono::steady_clock::now().time_since_epoch().count()));
  return engine;
}

inline uint64_t getint(uint64_t min, uint64_t max) {
  std::uniform_int_distribution<uint64_t> dist(min, max);
  return dist(p_getEngine());
}

inline double getdouble(double min, double max) {
  std::uniform_real_distribution<double> dist(
      min, std::nextafter(max, std::numeric_limits<double>::max()));
  return dist(p_getEngine());
}

inline bool getbool() {
  std::bernoulli_distribution dist(0.5); // 50/50 chance
  return dist(p_getEngine());
}

inline bool chance(double probability) {
  std::bernoulli_distribution dist(
      probability); // probability is from 0.0 to 1.0
  return dist(p_getEngine());
}

inline double normalDistribution(double mean, double stddev) {
  std::normal_distribution<double> dist(mean, stddev);
  return dist(p_getEngine());
}

template <typename T> inline void shuffle(std::vector<T> &vec) {
  std::shuffle(vec.begin(), vec.end(), p_getEngine());
}

inline void shuffle(std::string &word) {
  std::shuffle(word.begin(), word.end(), p_getEngine());
}

template <typename T> inline T getFrom(const std::vector<T> &vec) {
  if (vec.empty())
    return {};
  std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
  return vec[dist(p_getEngine())];
}

inline char getFrom(const std::string &word) {
  if (word.empty())
    return '\0';
  if (word.size() == 1)
    return word[0];

  return word[getint(0, word.size() - 1)];
}

/* PRIVATE members */

inline void seed(uint64_t seed) {
  if (seed == 0)
    seed = static_cast<size_t>(
        std::chrono::steady_clock::now().time_since_epoch().count());
  p_getEngine().seed(seed);
}

inline std::string generateUUID(bool add_hyphen) {
  const std::string charset = "0123456789abcdef";
  std::string uuid;
  constexpr int group_sizes[5] = {8, 4, 4, 4, 12};

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < group_sizes[i]; ++j) {
      uuid += getFrom(charset);
    }
    if (i != 4 && add_hyphen) {
      uuid += "-";
    }
  }
  return uuid;
}

} // namespace Random

#endif // random.hpp

/* Might add these functions in the future:

        Vector2 randomDirection2D();

        float bias(float min, float max, float power = 2.0f)

        bool skewedBool(double skew = 0.8)

        T weightedChoice(const std::vector<T>& items, const std::vector<double>&
   weights)

        int randomSign()
*/