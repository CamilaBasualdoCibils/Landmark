#pragma once
#include <pch.h>
class AppModule
{

public:
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
        int64_t priority = 0;
        std::function<void()> call;
        EngineCallInject(const std::string& name):inject_name(name) {};
    };
    
    virtual std::vector<EngineCallInject> GetInjections() = 0;
};