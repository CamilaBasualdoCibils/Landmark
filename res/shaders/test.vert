#version 450
#extension GL_GOOGLE_include_directive : enable

layout(location = 0) in vec3 inPosition;

layout(location = 1) in mat4 mat;
//layout(location = 1) in vec4 m0;
//layout(location = 2) in vec4 m1;
//layout(location = 3) in vec4 m2;
//layout(location = 4) in vec4 m3;


layout (location = 0) out vec3 outColor;

void main() {

    const vec3 pos = inPosition;
    vec4 gpos =mat*vec4(pos, 1.0);
    gl_Position =gpos;

    outColor = abs(gpos.xyz / gpos.w);
}