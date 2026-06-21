#include "../src/Input.hpp"
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

int main() {
  int age = Input::readUntilValid<int>("Age: ");
  auto rating = Input::readInRange<int>(1, 5, "Rating 1-5: ");
  auto continue_choice = Input::readOneOf<char>({'y', 'n'}, "Continue? y/n: ");
  int retries = Input::readWithDefault<int>(3, "Retries [3]: ");

  auto even = Input::readParsed<int>(
      [](const std::string &text) -> std::optional<int> {
        std::istringstream ss(text);
        int value;
        if ((ss >> value) && value % 2 == 0)
          return value;
        return std::nullopt;
      },
      "Even number: ");

  std::string password = Input::readSilent("Password: ");

  std::cout << "\nSummary\n";
  std::cout << "Age: " << age << "\n";
  std::cout << "Rating: " << (rating ? std::to_string(*rating) : "invalid")
            << "\n";
  std::cout << "Continue: "
            << (continue_choice ? std::string(1, *continue_choice) : "invalid")
            << "\n";
  std::cout << "Retries: " << retries << "\n";
  std::cout << "Even: " << (even ? std::to_string(*even) : "invalid") << "\n";
  std::cout << "Password length: " << password.size() << "\n";
}
