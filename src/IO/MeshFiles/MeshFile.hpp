
#pragma once
#include "mesh_obj.hpp"
#include "mesh_gltf.hpp"
#include <IO/File.h>

class MeshFile {


    public:
    /// @brief 
    /// @param file 
    /// @param output 
    /// @return successful?
    static bool ParseOBJ(File file, OBJ_File_content& output);
    static bool ParseGLTF(File file,GLTF_File_content& output);
};