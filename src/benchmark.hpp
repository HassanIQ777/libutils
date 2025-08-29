#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <functional>

struct CBenchmarkResult
{
	long double m_average_s; // seconds
	long double m_total_s;
};

class CBenchmark
{
  public:
	/*
	template <typename T>
	static double m_run(const size_t &runs, T(*));

	template <typename T, typename U>
	static double m_run(const size_t &runs, T (*)(U &u));
	*/

	template <typename Func, typename... Args>
	static CBenchmarkResult m_run(const size_t &runs, Func &&func, Args &&... args);
};
// end of class

template <typename Func, typename... Args>
CBenchmarkResult CBenchmark::m_run(const size_t &runs, Func &&func, Args &&... args)
{
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < runs; ++i)
	{
		std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	return {(static_cast<long double>(duration) / runs) / 1000.0f, static_cast<long double>(duration) / 1000.0f};
}

#endif // benchmark.hpp