/* 
license:
	All rights reserved to HassanIQ777
	You may:
		Use the code below, edit it or change it however you like, 
		but never republish it under a new name, 
		if so you may do it while crediting me.
		
Made on 2024 Nov 20th
Last update as of 2025 Jan 1
*/

#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>

class Stopwatch // i think this is pretty clear so it doesn't need documentation lol
{
  private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
	double total_elapsed = 0.0;  // keeps total elapsed time
	bool running = false;		// whether stopwatch is running

  public:
	Stopwatch() { start(); }
	void start()
	{
		if (!running)
		{
			running = true;
			start_time = std::chrono::high_resolution_clock::now();
		}
	}
	void pause()
	{
		if (running)
		{
			auto now = std::chrono::high_resolution_clock::now();
			total_elapsed += std::chrono::duration<double>(now - start_time).count();
			running = false;
		}
	}
	void resume() { start(); }
	double elapsed() const
	{
		if (running)
		{
			auto now = std::chrono::high_resolution_clock::now();
			return total_elapsed + std::chrono::duration<double>(now - start_time).count();
		}
		return total_elapsed;
	}
	void reset()
	{
		total_elapsed = 0.0;
		running = false;
	}
};

#endif // stopwatch.hpp