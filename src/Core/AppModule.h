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
        EngineCallPoints call_point = EngineCallPoints::INVALID;
        int64_t priority = 0;
        std::function<void()> call;
    };
    virtual std::vector<EngineCallInject> GetInjections() = 0;
};