#include <pch.h>
template <typename T>
class GPURef : public std::shared_ptr<T>
{

    template <typename... Args>
    GPURef(Args &&...args);
};


