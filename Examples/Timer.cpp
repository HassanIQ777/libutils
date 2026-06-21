#include "../src/Timer.hpp"
#include <chrono>
#include <iostream>
#include <thread>

void doWork(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main() {
  Timer timer;

  doWork(150);
  timer.pause();
  std::cout << "First chunk: " << timer.elapsed() << " seconds\n";

  doWork(100);
  std::cout << "Still paused: " << timer.elapsed() << " seconds\n";

  timer.resume();
  doWork(150);
  std::cout << "Total counted time: " << timer.elapsed() << " seconds\n";

  {
    ScopedTimer scoped("example", [](double seconds) {
      std::cout << "Scoped block: " << seconds << " seconds\n";
    });
    doWork(100);
  }
}
