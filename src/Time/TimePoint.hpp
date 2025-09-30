#pragma once
#include <pch.h>

struct TimePoint
{
    std::chrono::_V2::system_clock::time_point time_point;

    TimePoint(std::chrono::_V2::system_clock::time_point time_point) : time_point(time_point)
    {

    }
    TimePoint(std::filesystem::file_time_type time_point);

    std::string ToString() const;
    void FromString(const std::string& str);

};