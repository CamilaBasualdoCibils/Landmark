#pragma once
#include <pch.h>
class AppModule
{

public:
using Priority = int64_t;
    enum class EngineCallPoints
    {
        INVALID,
        START,
        UPDATE,
        RENDER,
        END

    };
    struct EngineCallInject
    {
        std::string inject_name;
        EngineCallPoints call_point = EngineCallPoints::INVALID;
        Priority priority = 0;
        std::function<void()> call;
        EngineCallInject(const std::string& name):inject_name(name) {};
    };
    //virtual priority GetInitPriority() = 0;
    virtual std::vector<EngineCallInject> GetInjections() = 0;
};