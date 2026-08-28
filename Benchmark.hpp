/* Part of https://github.com/HassanIQ777/libutils
Made on:     2025-Jul-20
Last update: 2025-Sep-06 */

#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>

#if defined(__x86_64__) || defined(__i386__) || defined(_M_X64) ||            \
    defined(_M_IX86)
#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#endif

struct BenchmarkResult {
  long double m_average; // unit is seconds
  long double m_total;
};

class Benchmark {
public:
  template <typename Func, typename... Args>
  static BenchmarkResult run(const size_t &runs, Func &&func, Args &&...args);
};
// end of class

template <typename Func, typename... Args>
inline BenchmarkResult Benchmark::run(const size_t &runs, Func &&func,
                                      Args &&...args) {
  if (runs == 0)
    return {0.0L, 0.0L};

  auto start = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < runs; ++i) {
    std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  return {(static_cast<long double>(duration) / runs) / 1'000'000'000.0L,
          static_cast<long double>(duration) / 1'000'000'000.0L};
}

// ======================
// CycleCounter
// ======================
class CycleCounter {
public:
  using cycles_t = uint64_t;

  // Start measuring
  void start() { begin = p_read_cycles(); }

  // Stop measuring
  void stop() { end = p_read_cycles(); }

  // Get elapsed cycles
  cycles_t cycles() const { return end - begin; }

  // Get elapsed nanoseconds (approx, uses CPU freq if available)
  uint64_t nanoseconds() const {
#if defined(__aarch64__) && (defined(__GNUC__) || defined(__clang__))
    uint64_t freq = p_read_cntfrq();
    if (freq == 0) {
      return cycles();
    }
    return (cycles() * 1'000'000'000ULL) / freq;
#else
    // fallback: assume 1 cycle ~ 1ns (rough!)
    return cycles();
#endif
  }

private:
  cycles_t begin = 0, end = 0;

  // -------- Platform-specific implementations --------
  static cycles_t p_read_cycles() {
#if defined(__x86_64__) || defined(__i386__) || defined(_M_X64) ||            \
    defined(_M_IX86)
#if defined(_MSC_VER)
    int cpu_info[4];
    unsigned int aux;
    __cpuid(cpu_info, 0); // serialize
    (void)__rdtsc();
    return __rdtscp(&aux);
#else
    unsigned int aux;
    asm volatile("cpuid\n\t" ::: "rax", "rbx", "rcx", "rdx"); // serialize
    (void)__rdtsc();
    return __rdtscp(&aux);
#endif
#elif defined(__aarch64__) && (defined(__GNUC__) || defined(__clang__))
    uint64_t cnt;
    asm volatile("mrs %0, cntvct_el0" : "=r"(cnt));
    return cnt;
#else
    // fallback: chrono
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
#endif
  }

#if defined(__aarch64__) && (defined(__GNUC__) || defined(__clang__))
  static uint64_t p_read_cntfrq() {
    uint64_t frq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(frq));
    return frq;
  }
#endif
};

#endif // benchmark.hpp
