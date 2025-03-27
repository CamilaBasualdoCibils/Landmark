#pragma once
#include "Editor/Debug.h"

namespace Landmark
{
	class  Time
	{
		static inline  Log logger = Log("Time");
		static inline std::chrono::time_point<std::chrono::system_clock> InitializationTime;
	public:
		static void Init();
		static double GetTime();
	};


}
