#pragma once
#include <pch.h>
#include "TimePoint.hpp"
class Time
{
static TimePoint Now();
static TimePoint Now_HighPrecision();
};