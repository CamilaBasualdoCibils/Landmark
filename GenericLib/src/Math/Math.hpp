//
// Created by camila on 11/9/25.
//

#ifndef LANDMARK_MATH_HPP
#define LANDMARK_MATH_HPP
#include "Libs/GLM.hpp"

using mat2 = glm::mat2;
using mat3 = glm::mat3;
using mat4 = glm::mat4;

template <glm::length_t L, typename T>
using vec = glm::vec<L, T>;

//Float
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

//Uint32
using uvec2 = glm::uvec2;
using uvec3 = glm::uvec3;
using uvec4 = glm::uvec4;

//Int32
using ivec2 = glm::ivec2;
using ivec3 = glm::ivec3;
using ivec4 = glm::ivec4;

using quat = glm::quat;


#endif //LANDMARK_MATH_HPP