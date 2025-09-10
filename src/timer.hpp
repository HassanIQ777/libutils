/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
Made on 2024 Nov 20th
Last update as of 2025 Sep 10
*/

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <functional>
#include <string>

class Timer // i think this is pretty clear so it doesn't need documentation lol
{
  private:
	std::chrono::time_point<std::chrono::high_resolution_clock> p_start_time;
	double p_total_elapsed = 0.0; // keeps total elapsed time
	bool p_running = false;		  // whether timer is running

  public:
	Timer() { p_start(); }
	Timer(const double &start_time) : p_total_elapsed(start_time) { p_start(); }

	void m_pause()
	{
		if (p_running)
		{
			auto now = std::chrono::high_resolution_clock::now();
			p_total_elapsed += std::chrono::duration<double>(now - p_start_time).count();
			p_running = false;
		}
	}

	void m_resume() { p_start(); }

	double m_elapsed() const
	{
		if (p_running)
		{
			auto now = std::chrono::high_resolution_clock::now();
			return p_total_elapsed + std::chrono::duration<double>(now - p_start_time).count();
		}
		return p_total_elapsed;
	}

	void m_reset()
	{
		p_total_elapsed = 0.0;
		p_running = false;
	}

	void m_restart()
	{
		p_total_elapsed = 0.0;
		// keep running
		p_running = true;
		p_start_time = std::chrono::high_resolution_clock::now();
	}

  private:
	void p_start()
	{
		if (!p_running)
		{
			p_running = true;
			p_start_time = std::chrono::high_resolution_clock::now();
		}
	}
};

class ScopedTimer
{
  public:
	using Callback = std::function<void(double)>;

	ScopedTimer(const std::string &name, Callback callback)
		: p_name(name), p_callback(std::move(callback)),
		  p_start_time(std::chrono::high_resolution_clock::now()) {}

	~ScopedTimer()
	{
		if (p_callback)
		{
			auto now = std::chrono::high_resolution_clock::now();
			double total_elapsed = std::chrono::duration<double>(now - p_start_time).count();

			p_callback(total_elapsed);
		}
	}

  private:
	std::string p_name;
	Callback p_callback;
	std::chrono::time_point<std::chrono::high_resolution_clock> p_start_time;
};

#endif // timer.hpp