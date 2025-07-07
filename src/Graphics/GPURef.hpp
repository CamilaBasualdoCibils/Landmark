#pragma once
#include <memory>
#include <pch.h>
#include <utility>
template <typename T> class GPURef : public std::shared_ptr<T>
{
  public:
    template <typename... Args> GPURef(Args &&...args) : std::shared_ptr<T>(std::forward<Args>(args)...)
    {
    }

    GPURef(const GPURef &rhs) = default;
    template <typename... Args> GPURef &Make(Args &&...args)
    {
        *this = MakeRef(std::forward<Args>(args)...);
        return *this;
    }
    template <typename... Args> static GPURef MakeRef(Args &&...args)
    {
        return (GPURef)std::make_shared<T>(std::forward<Args>(args)...);
    }
};
