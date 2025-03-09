#pragma once

#include <Debug/Debug.h>
#include <ECS/System.h>
class  TransformSystem : System
{
	static inline Log logger = Log("Transform");
	TransformSystem();
public:

	static void Update();
};
