#include "../Terminal.hpp"
#include "../funcs.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

// shared "how bright does this cell look" ramp — sparse to dense
inline const char kRamp[] = " .:-=+*#%@";
constexpr int kRampLen = sizeof(kRamp) - 2; // last usable index (excludes '\0')

inline char rampChar(float normalized01) {
  int idx = static_cast<int>(normalized01 * kRampLen);
  idx = std::clamp(idx, 0, kRampLen);
  return kRamp[idx];
}

// ---------------------------------------------------------------------------
// distance helper — aspect-corrected so circles look round, not egg-shaped
// ---------------------------------------------------------------------------
inline float dist2points(int x1, int y1, int x2, int y2) {
  int dx = x2 - x1;
  int dy = (y2 - y1) * 2; // terminal cells are ~2x taller than wide
  return std::sqrt(static_cast<float>(dx * dx + dy * dy));
}

// ---------------------------------------------------------------------------
// 1. Plasma — full-frame effect, every pixel overwritten every call
// ---------------------------------------------------------------------------
inline void plasma(Terminal &terminal, float t) {
  int w = terminal.getw(), h = terminal.geth();
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      float v = std::sin(x * 0.2f + t) + std::sin(y * 0.1f + t) +
                std::sin((x + y) * 0.15f + t) +
                std::sin(std::sqrt(float(x * x + y * y)) * 0.3f - t);
      // v ranges roughly [-4, 4]; squash into [0, 1] for the ramp
      float norm = (v + 4.0f) / 8.0f;
      terminal.setPixel(x, y, std::string(1, rampChar(norm)));
    }
  }
}

// ---------------------------------------------------------------------------
// 2. Ripple — full-frame effect, concentric waves from a center point
// ---------------------------------------------------------------------------
inline void ripple(Terminal &terminal, int cx, int cy, float t) {
  int w = terminal.getw(), h = terminal.geth();
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      float d = dist2points(cx, cy, x, y);
      float wave = std::sin(d * 0.3f - t * 3.0f);
      float norm = (wave + 1.0f) / 2.0f;
      terminal.setPixel(x, y, std::string(1, rampChar(norm)));
    }
  }
}

// ---------------------------------------------------------------------------
// 3. Fire — full-frame, but carries state (heat grid) between calls
// ---------------------------------------------------------------------------
struct FireState {
  std::vector<std::vector<int>> heat;
  FireState(int w, int h) : heat(h, std::vector<int>(w, 0)) {}
};

inline void fire(Terminal &terminal, FireState &state) {
  int w = terminal.getw(), h = terminal.geth();
  auto &heat = state.heat;
  if ((int)heat.size() != h || (int)heat[0].size() != w)
    return; // caller resized the terminal without rebuilding FireState — bail
            // rather than crash

  // seed fuel along the bottom row
  for (int x = 0; x < w; x++)
    heat[h - 1][x] = (rand() % 2) ? 255 : 0;

  // propagate heat upward with decay
  for (int y = 0; y < h - 1; y++) {
    for (int x = 1; x < w - 1; x++) {
      int sum =
          heat[y + 1][x - 1] + heat[y + 1][x] + heat[y + 1][x + 1] + heat[y][x];
      heat[y][x] = std::max(0, sum / 4 - (rand() % 3));
    }
  }

  static const char fireRamp[] = " .:;+=xX$&@";
  constexpr int fireRampLen = sizeof(fireRamp) - 2;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int idx = std::clamp(heat[y][x] * fireRampLen / 255, 0, fireRampLen);
      terminal.setPixel(x, y, std::string(1, fireRamp[idx]));
    }
  }
}

// ---------------------------------------------------------------------------
// 4. Matrix rain — sparse effect; needs an explicit fade pass to avoid smears
// ---------------------------------------------------------------------------
struct RainState {
  std::vector<int> dropY;
  RainState(int cols) : dropY(cols, 0) {}
};

// step every drop one row and glyph one column of rain forward
inline void matrixRain(Terminal &terminal, RainState &state) {
  int w = terminal.getw(), h = terminal.geth();
  if ((int)state.dropY.size() != w)
    return; // same guard as fire() — terminal resized underneath us

  for (int x = 0; x < w; x++) {
    if (rand() % 25 == 0)
      state.dropY[x] = 0; // occasionally spawn a fresh drop at the top

    int y = state.dropY[x];
    if (y < h) {
      char glyph = 'a' + rand() % 26;
      terminal.setPixel(x, y, std::string(1, glyph));
    }
    state.dropY[x] =
        (y + 1) % (h + 5); // +5 gives a gap of blank rows before it loops
  }
}

// ages every non-blank pixel one notch dimmer, eventually to blank —
// call this BEFORE matrixRain() each frame instead of terminal.clear()
inline void fadeTrail(Terminal &terminal) {
  static const std::string order = "@#$&Xx=+;:. "; // dense -> sparse -> blank
  int w = terminal.getw(), h = terminal.geth();
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      std::string p = terminal.getPixel(x, y);
      if (p.empty() || p == " ")
        continue;
      // letters from matrixRain aren't in `order` at all — treat any
      // unrecognized glyph as "freshest", i.e. one step above '@'
      auto pos = order.find(p[0]);
      size_t next = (pos == std::string::npos) ? 0 : pos + 1;
      if (next < order.size())
        terminal.setPixel(x, y, std::string(1, order[next]));
      else
        terminal.setPixel(x, y, " ");
    }
  }
}

enum Mode { PLASMA, RIPPLE, FIRE, RAIN, MODE_COUNT };

int main() {
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  Terminal terminal; // autoResizes to the real terminal on construction
  int w = terminal.getw(), h = terminal.geth();

  FireState fireState(w, h);
  RainState rainState(w);

  float t = 0.0f;
  int mode = PLASMA;
  int framesInMode = 0;
  constexpr int FRAMES_PER_MODE = 120; // ~6s at 50ms/frame before cycling

  while (true) {
    switch (mode) {
    case PLASMA:
      plasma(terminal, t);
      break;
    case RIPPLE:
      ripple(terminal, w / 2, h / 2, t);
      break;
    case FIRE:
      fire(terminal, fireState);
      break;
    case RAIN:
      fadeTrail(terminal); // age old glyphs instead of a hard clear
      matrixRain(terminal, rainState);
      break;
    }

    terminal.clearScreen();
    terminal.draw();

    t += 0.1f;
    framesInMode++;
    if (framesInMode >= FRAMES_PER_MODE) {
      framesInMode = 0;
      mode = (mode + 1) % MODE_COUNT;
      // fire/rain carry state across frames within a mode; starting a mode
      // fresh next time we cycle back keeps the effect from looking stale
      if (mode == FIRE)
        fireState = FireState(w, h);
      if (mode == RAIN)
        rainState = RainState(w);
    }

    funcs::msleep(50);
  }
}