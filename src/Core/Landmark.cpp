#include "Landmark.hpp"
#include "Graphics/Compositor/Compositor.hpp"
#include "Render/MeshParser.hpp"
#include "pch.h"
#include <Graphics/GraphicsEngine.hpp>
#include <Render/World.hpp>
#include <Render/WorldRenderer.hpp>
Landmark::Landmark(const LandmarkStartProperties &Properties)
{

    GraphicsEngine::Make(GraphicsEngineProperties{});

    std::shared_ptr<Mesh> meshes[2];
    for (int i = 0; i < 2; i++)
    {
        MeshParser::OBJ_File_content obj_content;
        MeshParser::Get().ParseOBJ(i ? "assets/models/cube.obj" : "assets/models/sphere.obj", obj_content);

        {
            std::vector<Mesh::Vertex> verticies;
            std::vector<uint32_t> indicies;
            for (const auto &oVert : obj_content.verticies)
            {
                Mesh::Vertex v;
                v.Position = oVert.position;
                if (oVert.color)
                    v.Color = *oVert.color;
                if (oVert.tex_coord)
                    v.UV = *oVert.tex_coord;
                verticies.push_back(v);
            }
            indicies.insert(indicies.end(), obj_content.indicies.begin(), obj_content.indicies.end());
            meshes[i] = std::make_shared<Mesh>(verticies, indicies);
        }
    }

    entt::entity e1 = World::Get().CreateEntity();
    World::Get().AddComponent<MeshDescriptor>(e1).mesh = meshes[0];
    entt::entity e2 = World::Get().CreateEntity();
    World::Get().AddComponent<MeshDescriptor>(e2).mesh = meshes[1];
    entt::entity camera = World::Get().CreateEntity();
    World::Get().AddComponent<Camera>(camera);
    World::Get().GetComponent<EntityInfo>(camera).Name = "Camera";
    World::Get().GetComponent<Transform>(camera).Position.z = 10;
    WorldRenderer::Check();
    while (!GraphicsEngine::Get().GetMainWindow()->GetShouldClose())
    {
        GraphicsEngine::Get().BeginFrame();

        Graphics::Compositor::Get().DrawDebug();
        World::Get().Update();
        GraphicsEngine::Get().EndFrame();
        GraphicsEngine::Get().Render();
    }
}
