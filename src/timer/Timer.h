#ifndef SKELETALANIMATION_TIMER_H
#define SKELETALANIMATION_TIMER_H

#include <chrono>

namespace Animation
{
	class Timer final
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

	public:
		Timer();

		~Timer();

		Timer(const Timer &) = delete;
	};
}

#endif //SKELETALANIMATION_TIMER_H
