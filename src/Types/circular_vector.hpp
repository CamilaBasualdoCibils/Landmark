

#pragma once
#include <pch.h>

template <typename T>
class circular_vector
{

    std::vector<T> vector;
    size_t Start,End;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
};