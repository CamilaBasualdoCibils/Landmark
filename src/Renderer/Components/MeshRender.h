#pragma once
#include "Entity/IComponentData.h"


struct MeshRender : public IComponentData<MeshRender>
{
	int a;

	MeshRender(ObjectID id)
		: IComponentData<MeshRender>(id)
	{
	}

	void ass()
	{
		std::cout << a;
	}
	//GraphicsPipeline* shader;
};

