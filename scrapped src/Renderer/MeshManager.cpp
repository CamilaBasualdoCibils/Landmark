#include "MeshManager.hpp"

std::shared_ptr<StaticMesh> MeshManager::FromMesh(const Mesh &mesh)
{
    std::shared_ptr<StaticMesh> sm = std::make_shared<StaticMesh>(mesh);

    static_meshes.push_back(sm);
}