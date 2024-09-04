#include "pch.h"
#include "Log.h"
#include "LogKeeper.h"
void Log::Print(const std::string &t)
{
    static auto const logKeeper = LogKeeper::GetInstance();
    std::cout << "[" << origin << "] " << t << '\n';
    logKeeper->Push(origin, t);
}