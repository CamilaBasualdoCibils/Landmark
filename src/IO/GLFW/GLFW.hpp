#pragma once
#include "misc/Singleton.hpp"
#include <pch.h>
#include <Graphics/Vulkan/VKExtension.hpp>
class GLFW :public Singleton<GLFW> {

    static void ErrorCallback(int code, const char* desc);
    public:
    GLFW();
    std::vector<VK::Extension> GetRequiredVKExtensions()const;
};