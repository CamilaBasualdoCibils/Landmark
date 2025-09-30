#include "Time.hpp"
TimePoint Time::Now()
{
    return TimePoint(std::chrono::system_clock::now());
}

TimePoint Time::Now_HighPrecision()
{
    return TimePoint(std::chrono::high_resolution_clock::now());
}
