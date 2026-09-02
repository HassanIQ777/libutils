#include "../src/LoadingBar.hpp"
#include "../src/Input.hpp"
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> toVec(const std::string &str) {
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string token;
  while (ss >> token) {
    result.push_back(token);
  }
  return result;
}

int main() {
  std::vector<std::string> v = {"🕐", "🕑", "🕒", "🕓", "🕔", "🕕",
                                "🕖", "🕗", "🕘", "🕙", "🕚", "🕛"};
  Loadingbar::Spinner lb{v, 1000 / (int)v.size()};

  Input::readline<int>();
  //   funcs::msleep(10000);
}