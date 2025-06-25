#pragma once
#include <ECS/Base/ComponentData.hpp>
// #include "Renderer/Mesh.h"
#include <Renderer/Mesh.h>
#include "IO/IO.h"
#include "Assets/Asset.hpp"
static const char MeshDescriptorName[] = "MeshDescriptor";

struct MeshDescriptor : public LMComponent<MeshDescriptor, MeshDescriptorName, false>
{
	MeshDescriptor(SceneID s_id, ObjectID id) : LMComponent(s_id, id) {}
	Mesh mesh;
	void DrawInspector() override
	{
		ImGui::Text("Vert Count:%li", mesh.VertexCount());
		ImGui::Text("Triangle Count:%li", mesh.IndexCount() / 3);

		ImGui::Separator();
		static bool valid_file = true;
		static std::string file;
		ImGui::InputText("obj file", &file);
		if (ImGui::Button("Load"))
		{
			Asset asset(file);
			if (!asset.Exists())
			{
				valid_file = false;
				return;
			}
			else valid_file = true;

			OBJ_File_content obj_file;
			MeshFile::ParseOBJ(asset, obj_file);
			std::vector<Mesh::Vertex> verticies;
			std::vector<uint32_t> indicies;
			for (const auto &v : obj_file.verticies)
			{
				Mesh::Vertex vert;
				vert.Position = v.position;
				verticies.push_back(vert);
			}
			for (const auto &i : obj_file.indicies)
				indicies.push_back(i);
			mesh = Mesh(verticies, indicies);
		}
		if (!valid_file)
			ImGui::TextColored(ImVec4(0.8, 0.5, 0.5, 1.0), "Invalid File");
	};
};
LMCOMPONENT_REGISTER(MeshDescriptor);