#include "../src/CLIParser.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  CLIParser cli(argc, argv);

  if (cli.hasFlag("--help") || cli.hasFlag("-h")) {
    std::cout << "Usage: " << cli.getArg(0)
              << " [--name NAME] [--count N] [--verbose]\n";
    return 0;
  }

  std::string name = cli.getValue("--name");
  std::string count = cli.getValue("--count");

  if (name.empty())
    name = "world";
  if (count.empty())
    count = "1";

  std::cout << "Hello, " << name << "\n";
  std::cout << "Count: " << count << "\n";

  if (cli.hasFlag("--verbose")) {
    std::cout << "Received " << cli.getArgc() << " arguments:\n";
    for (int i = 0; i < cli.getArgc(); ++i) {
      std::cout << "  [" << i << "] " << cli.getArg(i) << "\n";
    }
  }
}
