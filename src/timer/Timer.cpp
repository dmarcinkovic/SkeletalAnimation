#include <spdlog/spdlog.h>

#include "Timer.h"

namespace Animation
{
	Timer::Timer()
		: m_StartTime(std::chrono::high_resolution_clock::now())
	{

	}

	Timer::~Timer()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		long duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();

		spdlog::info("Time elapsed: {}ms", duration);
	}
}