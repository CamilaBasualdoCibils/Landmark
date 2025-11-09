#pragma once
#include <memory>
#include <pch.h>
#include <utility>
template <typename T> class Enable_GPURef_From_This;

template <typename T> class GPURef : public std::shared_ptr<T>
{
  public:
    using std::shared_ptr<T>::shared_ptr;

    template <typename... Args> GPURef(Args &&...args) : std::shared_ptr<T>(std::forward<Args>(args)...)
    {
    }
    template <typename... Args> GPURef &Make(Args &&...args)
    {
        *this = MakeRef(std::forward<Args>(args)...);
        return *this;
    }
     template <typename... Args> [[nodiscard]] static GPURef MakeRef(Args &&...args)
    {
        std::shared_ptr<T> ptr = std::make_shared<T>(std::forward<Args>(args)...);
        if constexpr (std::is_base_of<Enable_GPURef_From_This<T>, T>::value)
        {
            ptr->_internal_set_weak_self(ptr);
        }

        return GPURef(std::move(ptr));
    }
    GPURef() = default;
    GPURef(const GPURef &rhs) = default;
    GPURef(GPURef &&rhs) noexcept = default;
    GPURef &operator=(const GPURef &) = default;
    GPURef &operator=(GPURef &&) noexcept = default;

    explicit GPURef(std::shared_ptr<T> ptr) : std::shared_ptr<T>(std::move(ptr))
    {
    }
};
template <typename T> class GPURef_weak : public std::weak_ptr<T>
{
  public:
    using std::weak_ptr<T>::weak_ptr;

    template <typename... Args> GPURef_weak(Args &&...args) : std::weak_ptr<T>(std::forward<Args>(args)...)
    {
    }
    GPURef_weak() = default;
    GPURef_weak(const GPURef_weak &rhs) = default;
    GPURef_weak(GPURef_weak &&rhs) noexcept = default;
    GPURef_weak &operator=(const GPURef_weak &) = default;
    GPURef_weak &operator=(GPURef_weak &&) noexcept = default;

    explicit GPURef_weak(std::weak_ptr<T> ptr) : std::weak_ptr<T>(std::move(ptr))
    {
    }
};
template <typename T> class Enable_GPURef_From_This
{
    mutable std::weak_ptr<T> weak_self;

  public:
    GPURef<T> GPURef_from_this() const
    {
        return GPURef<T>(weak_self.lock());
    }

  private:
    friend class GPURef<T>;
    void _internal_set_weak_self(const std::shared_ptr<T> &self) const
    {
        weak_self = self;
    }
};