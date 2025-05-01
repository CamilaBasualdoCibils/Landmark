#pragma once
#include <ECS/Base/ComponentData.hpp>

static const char MeshRenderName[] = "MeshRender";
struct MeshRender : public LMComponent<MeshRender,MeshRenderName,true>
{
	int a;
	MeshRender(SceneID s_id,ObjectID id) : LMComponent(s_id,id) {}
	//GraphicsPipeline* shader;
	
	void DrawInspector() override {};
};
LMCOMPONENT_REGISTER(MeshRender);
