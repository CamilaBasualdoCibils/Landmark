#pragma once
#include "EditorObject.h"

class EditorObjectGroup : public EditorObject
{

	
	//         order    Tool
	//          V        V
	std::map<int64_t, EditorObject*> tools;


public:

	EditorObjectGroup(const std::string& _name);
	~EditorObjectGroup() {
		for (auto& t:tools) {
			delete t.second;
		}
	}
	template <typename T, typename... Args>
	T& PushObject(int64_t order = 0, Args&&... args)
	{
		if (tools.find(order) != tools.end())
		{
			for (auto rit = tools.rbegin(); rit != tools.rend(); ++rit) {
				auto nodeHandler = tools.extract((*rit).first);

				int64_t originalValue = nodeHandler.key();
				nodeHandler.key()++;
				tools.insert(std::move(nodeHandler));

				if (originalValue == order)
					break;  // Stop iterating after reaching the key
			}

		}
		tools.emplace(order, reinterpret_cast<EditorObject*>( new T(std::forward<Args>(args)...)));
		
		return *reinterpret_cast<T*>(tools.at(order));
	}
	template <typename T>
	T& GetObject(const std::string& name)
	{

		return  *TryGetObject<T*>(name).second;
	}
	template <typename T>
	std::pair<bool, T*> TryGetObject(const std::string& name)
	{
		for (auto it = tools.begin(); it != tools.end(); ++it)
		{
			if (it->second->GetLabel() == name)
				return std::make_pair( true,reinterpret_cast<T*>( it->second ));
		}
		return std::make_pair( false,nullptr );
	}

	EditorObjectGroup& GetOrPushGroup( const std::string& name)
	{
		auto result = TryGetObject<EditorObjectGroup>(name);
		if (!result.first)
			return PushObject<EditorObjectGroup>(tools.empty() ? 0: tools.rbegin()->first, name);

		return *result.second;
	}




public:

	void DrawTool() override;
	void DrawHandleWithoutMenu();
	void DrawHandle() override;
};
