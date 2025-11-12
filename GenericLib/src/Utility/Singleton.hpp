//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_SINGLETON_HPP
#define LANDMARK_SINGLETON_HPP
#pragma once
#include <memory>
#include "Utility/Assert.hpp"

template <typename Type> class Singleton
{
private:
    static inline std::unique_ptr<Type> Instance;


public:
    Singleton() = default;
    Singleton(const Singleton& o) = delete;

    template <typename... Args>
    static Type &Make(Args && ...args)
    {
        LASSERT(!Instance, "Singleton already exist");
        Type *rawPtr = static_cast<Type *>(::operator new(sizeof(Type)));
        Instance.reset(rawPtr);
        new (rawPtr) Type(std::forward<Args>(args)...);
        return *Instance;
    }
    
    [[nodiscard]] static Type &Get()
    {
        if constexpr (std::is_default_constructible_v<Type>) {
            if (!Instance)
            {
                Type *rawPtr = static_cast<Type *>(::operator new(sizeof(Type)));
                Instance.reset(rawPtr);
                new (rawPtr) Type();
            }
        }
        else {
            LASSERT(Instance, "Singleton Not Set");
        }
        return *Instance;
    }

    static void Ensure()
    {
        if constexpr (std::is_default_constructible_v<Type>) {
        Type &v = Get();
        }

        LASSERT(Instance, "Failed check");
    }
};
#endif //LANDMARK_SINGLETON_HPP