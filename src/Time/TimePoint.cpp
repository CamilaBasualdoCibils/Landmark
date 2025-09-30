#include "TimePoint.hpp"

TimePoint::TimePoint(std::filesystem::file_time_type file_tp)
{
    time_point = time_point_cast<std::chrono::system_clock::duration>(file_tp - decltype(file_tp)::clock::now() +
                                                                      std::chrono::system_clock::now());
}

std::string TimePoint::ToString() const
{
    auto s = std::chrono::time_point_cast<std::chrono::seconds>(time_point);
    auto frac = std::chrono::duration_cast<std::chrono::milliseconds>(time_point - s);

    std::time_t t = std::chrono::system_clock::to_time_t(s);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");                  // base
    oss << '.' << std::setw(3) << std::setfill('0') << frac.count(); // add ms
    return oss.str();
}

void TimePoint::FromString(const std::string &str)
{
    std::tm tm = {};
    char dot;
    int millis = 0;

    std::istringstream iss(str);
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (iss.peek() == '.')
    {
        iss >> dot >> millis; // read fractional part if present
    }

    std::time_t t = std::mktime(&tm);
    auto tp = std::chrono::system_clock::from_time_t(t);
    time_point = tp + std::chrono::milliseconds(millis);
}
