#pragma once
#include <pch.h>
template <typename Type, typename ConstructorArguments = void>
class Singleton
{
private:
    static inline std::unique_ptr<Type> Instance;

public:
    // if constructor arguments
    template <typename T = ConstructorArguments, std::enable_if_t<!std::is_void_v<T>, int> = 0>
    [[nodiscard]] static Type &Get()
    {
        LASSERT(Instance, "Singleton Not Set");
        return *Instance;
    }
    
    template <typename T = ConstructorArguments, std::enable_if_t<!std::is_void_v<T>, int> = 0>
    static Type &Make(const T & args)
    {
        LASSERT(!Instance, "Singleton already exist");
        Type* rawPtr = static_cast<Type*>(::operator new(sizeof(Type)));
        Instance.reset(rawPtr);
        new (rawPtr) Type(args);
        return *Instance;
    }

    // if no constructor arguments
        template <typename T = ConstructorArguments, std::enable_if_t<std::is_void_v<T>, int> = 0>
    [[nodiscard]] static Type &Get()
    {
        if (!Instance)
            Instance = std::make_unique<Type>();
        return *Instance;
    }

    static void Check()
    {
        Type& v = Get();
        LASSERT(Instance, "Failed check");
    }
};