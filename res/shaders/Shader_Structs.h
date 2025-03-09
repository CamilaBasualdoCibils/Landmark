//
// Created by camila on 10/27/24.
//

#ifndef SHADER_STRUCTS_H
#define SHADER_STRUCTS_H

#ifdef __cplusplus
#define UNIFORM_BLOCK(x) struct
#else
#define UNIFORM_BLOCK(BINDING) layout(binding = BINDING) uniform
#endif

UNIFORM_BLOCK(0) Model_UBO {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;
#endif //SHADER_STRUCTS_H
