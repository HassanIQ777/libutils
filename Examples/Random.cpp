#include "../Random.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
  Random::seed(12345);

  std::cout << "Integer 1-100: " << Random::integer(1, 100) << "\n";
  std::cout << "Double 0-1: " << Random::real(0.0, 1.0) << "\n";
  std::cout << "Coin flip: " << (Random::boolean() ? "heads" : "tails") << "\n";
  std::cout << "25% chance: " << (Random::chance(0.25) ? "hit" : "miss")
            << "\n";
  std::cout << "UUID: " << Random::generateUUID(true) << "\n";

  std::vector<std::string> names = {"Ada", "Grace", "Linus", "Dennis"};
  Random::shuffle(names);

  std::cout << "Shuffled names:";
  for (const auto &name : names)
    std::cout << ' ' << name;
  std::cout << "\n";

  std::string letters = "abcdef";
  Random::shuffle(letters);
  std::cout << "Shuffled letters: " << letters << "\n";
  std::cout << "Random name: " << Random::getFrom(names) << "\n";
}
