#pragma once
#include <pch.h>
struct OBJ_File_content {
    struct Vertex {
        vec3 position;
        std::optional<vec2> tex_coord;
        std::optional<vec4> color;
    };
    std::vector<Vertex> verticies;
    std::vector<uint64_t> indicies;
};