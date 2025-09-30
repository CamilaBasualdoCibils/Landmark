#pragma once
#include <pch.h>

template <size_t dim, typename type = float> struct AABB
{
    using vectype = vec<dim, type>;
    vectype Min, Max;

    AABB(vectype min, vectype max) : Min(min), Max(max)
    {
    }

    vec<dim, float> Center() const
    {
        return vec<dim, float>(Min + Max) / vec<dim, float>(2.0f);
    }
    vectype Size() const
    {
        return Max - Min;
    }
    vec<dim, int32> SignFromCenter(vec<dim, float> pos) const
    {
        const vec<dim, float> center = Center();
        return glm::sign(pos - center);
    }

    template <typename T> bool PointAABB(vec<dim, T> point) const
    {
        for (size_t i = 0; i < dim; ++i)
        {
            if (point[i] < Min[i] || point[i] > Max[i])
                return false;
        }
        return true;
    }
};

using AABB2 = AABB<2, float>;
using AABB3 = AABB<3, float>;
using AABB2i = AABB<2, uint32>;
using AABB3i = AABB<3, uint32>;