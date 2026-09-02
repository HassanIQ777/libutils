#include "../src/Binary.hpp"
#include "../src/Tokenizer.hpp"
#include "../src/numutils.hpp"
#include "../src/strutils.hpp"
#include <iostream>

int main() {
  std::string messy = "  --libutils-demo--  ";
  std::string trimmed = strutils::trim(messy);
  std::string clean = strutils::trim_chars(trimmed, "-");

  std::cout << "Original: [" << messy << "]\n";
  std::cout << "Clean: [" << clean << "]\n";
  std::cout << "Centered: [" << strutils::center(clean, 20, ".") << "]\n";

  std::cout << "Grouped: " << numutils::groupDigits(123456789) << "\n";
  std::cout << "Bytes: " << numutils::bytes(12345678) << "\n";
  std::cout << "Percent: " << numutils::percent(0.853) << "\n";
  std::cout << "Duration: "
            << numutils::durationToString(numutils::duration(3661)) << "\n";

  uint32_t flags = Binary::clear();
  flags = Binary::set(flags, 1);
  flags = Binary::set(flags, 3);
  flags = Binary::toggle(flags, 1);

  std::cout << "Flags: " << numutils::binary(flags, true) << "\n";
  std::cout << "Bit 3 set? " << (Binary::get(flags, 3) ? "yes" : "no")
            << "\n";

  Tokenizer tokens("open save close --force");
  std::cout << "Has save? " << (tokens.matchExact("save") ? "yes" : "no")
            << "\n";
  std::cout << "Has forcr? " << (tokens.match("forcr") ? "yes" : "no")
            << "\n";
}
