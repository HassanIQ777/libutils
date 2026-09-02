#include "../src/Table.hpp"
#include <iostream>

int main() {
  Table scores;
  scores.setHeader("Name", "Score", "Grade");
  scores.addRow("Ada", 98, "A+");
  scores.addRow("Grace", 95, "A");
  scores.addRow("Linus", 91, "A-");

  scores.setTrailingSpaces(4);
  scores.setFillerChar('=');

  std::cout << scores << "\n";

  const std::string path = "/tmp/libutils_scores.csv";
  scores.exportCSV(path);
  std::cout << "Exported: " << path << "\n\n";

  Table imported;
  imported.importCSV(path);
  std::cout << "Imported again:\n" << imported;
}
