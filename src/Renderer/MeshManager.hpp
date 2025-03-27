
#pragma once
#include <pch.h>
#include "StaticMesh.hpp"
class MeshManager {

     std::vector<std::shared_ptr<StaticMesh>> static_meshes;



     std::shared_ptr<StaticMesh> FromMesh(const Mesh& mesh);
};