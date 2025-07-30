#pragma once
#include <Graphics/Vulkan/Buffer/VKBuffer.hpp>
class Mesh
{
  public:
    struct Vertex
    {
        vec3 Position;
        vec3 Normal;
        vec4 Color;
        vec2 UV;
        vec3 Tangent;
        vec3 Bitangent;
    };
    using IndexType = uint32_t;

  private:
    std::vector<Vertex> verticies;
    std::vector<IndexType> indicies;
    GPURef<VK::Buffer> VertexBuffer, IndexBuffer;

  public:
    Mesh(std::vector<Vertex> verts = {}, std::vector<IndexType> indicies = {}) : verticies(verts), indicies(indicies)
    {
        const size_t VertDataSize = VertexCount() * sizeof(Vertex), IndexDataSize = IndexCount() * sizeof(IndexType);

        VertexBuffer.Make(
            VK::BufferProperties{.Size = VertDataSize,
                                 .UsageFlags = {VK::BufferUsage::eVertexBuffer, VK::BufferUsage::eTransferDst,
                                                VK::BufferUsage::eTransferSrc}});
        std::memcpy(VertexBuffer->Map(), verticies.data(), VertDataSize);
        VertexBuffer->Unmap();

        IndexBuffer.Make(
            VK::BufferProperties{.Size = IndexDataSize,
                                 .UsageFlags = {VK::BufferUsage::eIndexBuffer, VK::BufferUsage::eTransferDst,
                                                VK::BufferUsage::eTransferSrc}});
        std::memcpy(IndexBuffer->Map(), indicies.data(), IndexDataSize);
        IndexBuffer->Unmap();
    }
    size_t IndexCount() const
    {
        return indicies.size();
    }
    size_t VertexCount() const
    {
        return verticies.size();
    }
    const std::vector<Vertex> &GetVerticies() const
    {
        return verticies;
    }
    const std::vector<IndexType> &GetIndicies() const
    {
        return indicies;
    }

    ~Mesh()
    {
    }

	GPURef<VK::Buffer> GetVertexBuffer() const {return VertexBuffer;}
	GPURef<VK::Buffer> GetIndexBuffer() const {return IndexBuffer;}
};
