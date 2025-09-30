/* Part of https://github.com/HassanIQ777/libutils
Made on: 	2025-Jul-20
Last update: 2025-Sep-06 */

#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <cstdint>
#include <chrono>
#include <functional>

#if defined(__x86_64__) || defined(__i386__)
#include <x86intrin.h>
#endif

struct CBenchmarkResult
{
	long double m_average; // seconds
	long double m_total;
};

class CBenchmark
{
  public:
	template <typename Func, typename... Args>
	static CBenchmarkResult m_run(const size_t &runs, Func &&func, Args &&... args);
};
// end of class

template <typename Func, typename... Args>
CBenchmarkResult CBenchmark::m_run(const size_t &runs, Func &&func, Args &&... args)
{
	if (runs == 0)
		return {0.0L, 0.0L};

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < runs; ++i)
	{
		std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	return {(static_cast<long double>(duration) / runs) / 1'000'000'000.0L,
			static_cast<long double>(duration) / 1'000'000'000.0L};
}

// ======================
// CycleCounter
// ======================
class CycleCounter
{
  public:
	using cycles_t = uint64_t;

	// Start measuring
	void m_start()
	{
		begin = p_read_cycles();
	}

	// Stop measuring
	void m_stop()
	{
		end = p_read_cycles();
	}

	// Get elapsed cycles
	cycles_t m_cycles() const
	{
		return end - begin;
	}

	// Get elapsed nanoseconds (approx, uses CPU freq if available)
	uint64_t m_nanoseconds() const
	{
#if defined(__aarch64__)
		uint64_t freq = p_read_cntfrq();
		return (m_cycles() * 1'000'000'000ULL) / freq;
#else
		// fallback: assume 1 cycle ~ 1ns (rough!)
		return m_cycles();
#endif
	}

  private:
	cycles_t begin = 0, end = 0;

	// -------- Platform-specific implementations --------
	static cycles_t p_read_cycles()
	{
#if defined(__x86_64__) || defined(__i386__)
		unsigned int aux;
		uint64_t r1, r2;
		asm volatile("cpuid\n\t" ::
						 : "rax", "rbx", "rcx", "rdx"); // serialize
		r1 = __rdtsc();
		asm volatile("cpuid\n\t" ::
						 : "rax", "rbx", "rcx", "rdx");
		r2 = __rdtscp(&aux);
		return r2; // safe value
#elif defined(__aarch64__)
		uint64_t cnt;
		asm volatile("mrs %0, cntvct_el0"
					 : "=r"(cnt));
		return cnt;
#else
		// fallback: chrono
		auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
#endif
	}

#if defined(__aarch64__)
	static uint64_t p_read_cntfrq()
	{
		uint64_t frq;
		asm volatile("mrs %0, cntfrq_el0"
					 : "=r"(frq));
		return frq;
	}
#endif
};

#endif // benchmark.hpp