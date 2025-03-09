#version 450
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 m0;
layout(location = 2) in vec4 m1;
layout(location = 3) in vec4 m2;
layout(location = 4) in vec4 m3;


layout (location = 0) out vec3 outColor;

void main() {
    mat4 mat = mat4(m0,m1,m2,m3);
    const vec3 pos = inPosition;
    gl_Position =mat*vec4(pos, 1.0);

    outColor = abs(gl_Position.xyz / gl_Position.w);
}