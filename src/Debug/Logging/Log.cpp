#include "pch.h"
#include "Log.h"


#include "LogKeeper2.h"
void Log::Print(const std::string &t)
{
    static auto const logKeeper = LogKeeper2::GetInstance();
    std::cerr << "[" << origin << "] " << t << '\n';



    logKeeper->PushLog(origin, t);

}