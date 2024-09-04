#pragma once
#include <Entity/EntityManager.h>
#include <Debug/Debug.h>
#include <Entity/System.h>
class  TransformSystem : System
{
	static inline Log logger = Log("Transform");
	TransformSystem();
public:

	static void Update();
};
