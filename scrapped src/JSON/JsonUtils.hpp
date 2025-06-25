#include <pch.h>

namespace JsonUtils {


enum JsonVecFormat
{
    XYZW,
    RGBA
};
const static inline std::unordered_map<JsonVecFormat, std::vector<std::string>> format_components = {
    {XYZW, {"X", "Y", "Z", "W"}},
    {RGBA, {"R", "G", "B", "A"}}

};
template <int L, typename T>
Json VecToJson(vec<L, T> v, JsonVecFormat format);
template <int L,typename T>
vec<L, T> VecFromJson(const Json &j, JsonVecFormat format);
template <int L,typename T>
inline Json VecToJson(vec<L, T> v, JsonVecFormat format)
{
    Json json;
    for (int i = 0; i < L; i++)
        json[format_components.at(format)[i]] = v[i];
    return json;
}

template <int L,typename T>
inline vec<L, T> VecFromJson(const Json &j, JsonVecFormat format)
{
    vec<L, T> vec;
    for (int i = 0; i < L; i++)
        vec[i] = j[format_components.at(format)[i]].get<T>();
        return vec;
}


auto Vec2FromJson = [](const Json &j, JsonVecFormat format) {return VecFromJson<2,float>(j,format);};
auto Vec2ToJson = [](const vec2 &v, JsonVecFormat format) {return VecToJson<2,float>(v,format);};

auto Vec3FromJson = [](const Json &j, JsonVecFormat format) {return VecFromJson<3,float>(j,format);};
auto Vec3ToJson = [](const vec3 &v, JsonVecFormat format) {return VecToJson<3,float>(v,format);};

auto Vec4FromJson = [](const Json &j, JsonVecFormat format) {return VecFromJson<4,float>(j,format);};
auto Vec4ToJson = [](const vec4 &v, JsonVecFormat format) {return VecToJson<4,float>(v,format);};
}