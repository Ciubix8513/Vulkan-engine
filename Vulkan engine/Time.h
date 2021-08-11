#ifndef _TIME_H_
#include <chrono>

class Time
{
public:
		
	void Update();
	std::chrono::time_point<std::chrono::system_clock> GetStaringTime();
	double GetDeltaT();
private:
	std::chrono::time_point<std::chrono::system_clock> m_begin;
	std::chrono::time_point<std::chrono::system_clock> m_lastTime;
	double m_deltaT;
	friend class System;
	
};
#endif

