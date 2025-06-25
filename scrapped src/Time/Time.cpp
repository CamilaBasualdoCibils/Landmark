#include "pch.h"
#include "Time.h"

void Landmark::Time::Init()
{
	InitializationTime = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(InitializationTime);
	//std::tm localTime;
	//localtime_r(&localTime,& t);
	std::stringstream ss;
	ss << "Initialized at " <<"Some time, fix this, make this show a clock of smth";
	logger.Debug(ss.str());
}

double Landmark::Time::GetTime()
{ return glfwGetTime(); }
