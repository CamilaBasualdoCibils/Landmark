#include <pch.h>
#include <chrono>
using milliseconds = std::chrono::milliseconds;
using seconds = std::chrono::seconds;
using minutes = std::chrono::minutes;
using hours = std::chrono::hours;

template<typename T>
T time_cast(const T&);