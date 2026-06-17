#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

/* you can use these as loading bars:
{"⠋", "⠙", "⠹", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
{"🕐","🕑","🕒","🕓","🕔","🕕","🕖","🕗","🕘","🕙","🕚","🕛"}
{"|", "/", "-", "\\"}
{".  ", ".. ", "..."}
{"▏","▎","▍","▌","▋","▊","▉"}
{"✶", "✸", "✹", "✺", "✹", "✸"}
{"(●    )", "( ●   )", "(  ●  )", "(   ● )", "(    ●)", "(   ● )", "(  ●  )",
"(●   )"}
{"ᗧ···", " ᗧ··", "  ᗧ·", "   ᗧ", "  ᗧ·", " ᗧ··"}
{"|", "!", "I", "1", "l", "|"}
{"[          ]", "[=         ]", "[==        ]", "[===       ]","[====
]","[=====     ]", "[======    ]", "[=======   ]", "[========  ]", "[=========
]","[==========]"}

@ these ones are different:
@ "▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂"
@ "← ↖ ↑ ↗ → ↘ ↓ ↙"
@ "▉ ▊ ▋ ▌ ▍ ▎ ▏ ▎ ▍ ▌ ▋ ▊"
@ "▖ ▘ ▝ ▗"
@ "┤ ┘ ┴ └ ├ ┌ ┬ ┐"
@ "◢ ◣ ◤ ◥"
@ "◰ ◳ ◲ ◱"
@ "⣾ ⣽ ⣻ ⢿ ⡿ ⣟ ⣯ ⣷"
@ "⠁ ⠂ ⠄ ⡀ ⢀ ⠠ ⠐ ⠈"
@ "◴ ◷ ◶ ◵"
@ "◐ ◓ ◑ ◒"
@ "◡◡ ⊙⊙ ◠◠"
@ "⣾ ⣽ ⣻ ⢿ ⡿ ⣟ ⣯ ⣷"
@ "⠁ ⠂ ⠄ ⡀ ⢀ ⠠ ⠐ ⠈"

*/

namespace Loadingbar {

class Spinner {
  std::atomic<bool> done;
  std::thread thread;

public:
  Spinner(const std::vector<std::string> &loading_bar, int sleep_duration,
          const std::string &msg = "Loading")
      : done(false),
        thread([this, loading_bar, sleep_duration, msg]() { // capture by value
          size_t i = 0;
          while (!done.load(std::memory_order_acquire)) {
            std::cout << "\033[2K" << "\r"
                      << loading_bar[i % loading_bar.size()] << " " << msg
                      << std::flush;
            std::this_thread::sleep_for(
                std::chrono::milliseconds(sleep_duration));
            i++;
          }
          std::cout << "\r✓ " << msg << " done!          \n" << std::flush;
        }) {}

  void stop() {
    done = true;
    if (thread.joinable())
      thread.join();
  }

  ~Spinner() { stop(); }

  Spinner(const Spinner &) = delete;
  Spinner &operator=(const Spinner &) = delete;
};

// class

} // namespace Loadingbar