#include "Time.h"


void Time::Update()
{
	auto time = std::chrono::system_clock::now();
	std::chrono::duration<double> SecSinceLastFrame = time - m_lastTime;
	m_deltaT = SecSinceLastFrame.count();
	m_lastTime = time;
}

std::chrono::time_point<std::chrono::system_clock> Time::GetStaringTime()
{
    return m_begin;
}

double Time::GetDeltaT()
{
    return m_deltaT;
}
