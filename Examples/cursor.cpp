#include <chrono>
#include <iostream>
#include <string>
#include <thread>

// lines are tracked bottom-up: line 1 = directly above cursor
void updateLine(int linesFromBottom, const std::string &content) {
  std::cout << "\033[" << linesFromBottom << "A" // jump up
            << "\r\033[2K"                       // nuke the line
            << content                           // write fresh content
            << "\033[" << linesFromBottom << "B" // jump back down
            << std::flush;
}

int main() {
  // first, PRINT the lines to reserve space on screen
  // the cursor ends up below all of them — that's the anchor point
  std::cout << "Status:   (waiting)\n" // line 3 from bottom (linesFromBottom=3)
            << "Progress: (waiting)\n" // line 2 from bottom
            << "Last:     (waiting)\n" // line 1 from bottom
            << std::flush;             // cursor is HERE (below all 3)

  for (int i = 0; i <= 10; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    updateLine(3, "Status:   running (" + std::to_string(i * 10) + "%)");
    updateLine(2, "Progress: [" + std::string(i, '#') +
                      std::string(10 - i, '-') + "]");
    updateLine(1, "Last:     processed item #" + std::to_string(i));
  }

  // move cursor BELOW the display block so normal output doesn't clobber it
  std::cout << "\nAll done!\n";
}