#pragma once

class Mesh
{
public:
	struct Vertex
	{
		vec3 Position;
		vec3 Normal;
		vec3 Color;
		vec2 UV;
		vec3 Tangent;
		vec3 Bitangent;
	};
	using IndexType = uint32_t;

private:
	std::vector<Vertex> verticies;
	std::vector<IndexType> indicies;

public:
	Mesh(std::vector<Vertex> verts = {}, std::vector<IndexType> indicies = {});
	size_t IndexCount() { return indicies.size(); }

	~Mesh();
};
