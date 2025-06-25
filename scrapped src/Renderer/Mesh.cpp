#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verts, std::vector<IndexType> indicies) : verticies(verts), indicies(indicies)
{
}

Mesh::~Mesh()
{
}
