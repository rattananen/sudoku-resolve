#include "time.hpp"

namespace jkk::time {
	
	void Stopwatch::start()
	{
		m_start = Stopwatch::clock_type::now();
	}
	Stopwatch::duration Stopwatch::stop()
	{
		return Stopwatch::clock_type::now() - m_start;
	}
}