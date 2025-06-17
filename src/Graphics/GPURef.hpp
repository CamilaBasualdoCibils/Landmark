#include <pch.h>
template <typename T>
class GPURef : public std::shared_ptr<T>
{

    template <typename... Args>
    inline GPURef(Args &&...args);
};

template <typename T,typename... Args>
GPURef<T>::GPURef(Args &&...args):std::shared_ptr<T>(std::forward(args)...)
{

}
