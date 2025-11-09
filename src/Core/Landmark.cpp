#include "Landmark.hpp"


#include "pch.h"
#include <Graphics/GraphicsEngine.hpp>
#include "AssetControl/FileParserRegistry.hpp"
#include "AssetControl/AssetRegistry.hpp"
#include "Render/Terrain/Atlas.hpp"
#include "Render/Compositor/CompositorTest.hpp"
#include "Render/Renderer/Renderer.hpp"
#include "Platform/Window/WindowManager.hpp"
#include "Entity/World.hpp"
Landmark::Landmark(const LandmarkStartProperties &Properties)
{
    GraphicsEngine::Make(GraphicsEngineProperties{});
    Atlas::Check();
    CompositeTestSimple();
    FileParserRegistry::Check();
    AssetRegistry::Check();
    //Renderer::Check();
    //std::shared_ptr<Mesh> meshes[2];
    //for (int i = 0; i < 2; i++)
    //{
    //    MeshParser::OBJ_File_content obj_content;
    //    MeshParser::Get().ParseOBJ(i ? "assets/models/cube.obj" : "assets/models/sphere.obj", obj_content);
//
    //    {
    //        std::vector<Mesh::Vertex> verticies;
    //        std::vector<uint32_t> indicies;
    //        for (const auto &oVert : obj_content.verticies)
    //        {
    //            Mesh::Vertex v;
    //            v.Position = oVert.position;
    //            if (oVert.color)
    //                v.Color = *oVert.color;
    //            if (oVert.tex_coord)
    //                v.UV = *oVert.tex_coord;
    //            verticies.push_back(v);
    //        }
    //        indicies.insert(indicies.end(), obj_content.indicies.begin(), obj_content.indicies.end());
    //        meshes[i] = std::make_shared<Mesh>(verticies, indicies);
    //    }
    //}
//
    //Entity e1 = World::Get().CreateEntity();
    //e1.AddComponent<MeshDescriptor>().mesh = meshes[0];
    //Entity e2 = World::Get().CreateEntity();
    //e2.AddComponent<MeshDescriptor>().mesh = meshes[1];
    //Entity camera1 = World::Get().CreateEntity();
    //const CameraComponent &cam1 = camera1.AddComponent<CameraComponent>();
    //camera1.GetInfo().Name = "Camera 1";
    //camera1.GetComponent<TransformComponent>().Position.z = 10;
//
    //Entity camera2 = World::Get().CreateEntity();
    //const CameraComponent &cam2 = camera2.AddComponent<CameraComponent>();
    //camera2.GetInfo().Name = "Some other camera";
//
    //auto &camera2Transform = camera2.GetComponent<TransformComponent>();
    //camera2Transform.Position = {2, 1, 5};
    //camera2Transform.Rotation =
    //    glm::quatLookAt(e1.GetComponent<TransformComponent>().Position - camera2Transform.Position, vec3(0, 1, 0));
    while (!Platform::WindowManager::Get().GetMainWindow()->GetShouldClose())
    {
        GraphicsEngine::Get().BeginFrame();
        //Graphics::Compositor::Get().DrawDebug();
        World::Get().Update();
        GraphicsEngine::Get().EndFrame();
        GraphicsEngine::Get().Render();
    }
}
