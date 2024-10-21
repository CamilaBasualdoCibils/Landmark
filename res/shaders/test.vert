#version 450


vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);
vec3 color[] = {
{1,0,0},
{0,1,0},
{0,0,1}
};

layout (location = 0) out vec3 outColor;

void main() {
    const vec3 pos = vec3(positions[gl_VertexIndex], 0.0);
    gl_Position = vec4(pos, 1.0);

    outColor = pos;
}