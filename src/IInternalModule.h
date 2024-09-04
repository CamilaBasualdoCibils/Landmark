#pragma once

namespace Landmark::Systems
{
	template<class T>
	class  IInternalModule
	{

		
	protected:
		IInternalModule()
		{
			Instance = reinterpret_cast<T*>( this);
		}
	public:
		IInternalModule(const IInternalModule& other) = delete;
		IInternalModule& operator=(const IInternalModule& other) = delete;
		static inline T* Instance = nullptr;
		template<typename... ARGS>
		static T* Initialize(ARGS&&... args)
		{
			if (Instance)
				throw std::runtime_error("Internal Module Tried to be initialized twice!");

			Instance = new T(std::forward<ARGS>(args)...);
			
			return Instance;
		}


		static T* GetInstance()
		{
			
			if (!Instance) throw std::runtime_error("Internal Module has not been initialized. call Initialize() first");

			return Instance;
		}




	
	};


}
