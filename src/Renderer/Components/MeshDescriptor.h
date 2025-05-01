#pragma once
#include <ECS/Base/ComponentData.hpp>
//#include "Renderer/Mesh.h"
#include <Renderer/Mesh.h>
static const char MeshDescriptorName[] = "MeshDescriptor";

struct MeshDescriptor :public LMComponent<MeshDescriptor,MeshDescriptorName,false>
{
	MeshDescriptor(SceneID s_id,ObjectID id) : LMComponent(s_id,id) {}
	Mesh mesh;
	void DrawInspector() override {};

};
LMCOMPONENT_REGISTER(MeshDescriptor);