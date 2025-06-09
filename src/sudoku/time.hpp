#pragma once

#include <chrono>

namespace jkk::time {
	struct Stopwatch {
		using clock_type = std::chrono::high_resolution_clock;
		using time_point = clock_type::time_point;
		using duration = std::chrono::duration<double, std::milli>;

		void start();

		duration stop();
		
		time_point m_start;
	};
}