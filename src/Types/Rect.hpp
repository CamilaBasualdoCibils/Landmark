#pragma once
#include <pch.h>
template <typename T, int N>
struct Rect {
    using vec_type = vec<N,T>;
    vec_type min,max;
    vec_type size() {return max-min;}
    

};
using Rect2D = Rect<float,2>;
using Rect3D = Rect<float,3>;

using IRect2D = Rect<uint32_t,2>;
using IRect3D = Rect<uint32_t,3>;
