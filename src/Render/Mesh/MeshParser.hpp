#pragma once
#include "Mesh.hpp"
#include <misc/Singleton.hpp>
#include <pch.h>
class MeshParser : public Singleton<MeshParser>
{
  public:
    struct OBJ_File_content
    {
        struct Vertex
        {
            vec3 position;
            std::optional<vec2> tex_coord;
            std::optional<vec4> color;
        };
        std::vector<Vertex> verticies;
        std::vector<uint64_t> indicies;
    };
    bool ParseOBJ(std::string filename, OBJ_File_content &output);
};