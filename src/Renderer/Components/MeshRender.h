#pragma once
#include "Entity/IComponent.h"


struct MeshRender : public IComponent<MeshRender>
{
	int a;

	MeshRender(EntityID id)
		: IComponent<MeshRender>(id)
	{
	}

	void ass()
	{
		std::cout << a;
	}
	//GraphicsPipeline* shader;
};

