#pragma once
namespace Landmark
{
	class  IAttachableModule
	{
	public:
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void PreRender()=0;
		virtual void Exit()=0;
	
	};
}


