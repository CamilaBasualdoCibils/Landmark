#pragma once
#include "EditorTool.hpp"

class EditorObjectGroup : public EditorTool
{

	
	//         order    Tool
	//          V        V
	std::map<int64_t, std::shared_ptr<EditorObject>> tools;


public:

	EditorObjectGroup(const std::string& _name);
	~EditorObjectGroup() {
	}
	template <typename T, typename... Args>
	std::shared_ptr<T> PushObject(int64_t order = 0, Args&&... args)
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
		auto new_t = std::make_shared<T>(std::forward<Args>(args)...);
		tools.emplace(order, std::reinterpret_pointer_cast<EditorObject>(new_t ));
		
		return new_t;
	}
	template <typename T>
	std::shared_ptr<T> GetObject(const std::string& name)
	{

		return TryGetObject<T>(name).second;
	}
	template <typename T>
	std::pair<bool, std::shared_ptr<T>> TryGetObject(const std::string& name)
	{
		for (auto it = tools.begin(); it != tools.end(); ++it)
		{
			if (it->second->GetLabel() == name)
				return std::make_pair( true,std::reinterpret_pointer_cast<T>(it->second));
		}
		return std::make_pair( false,nullptr );
	}

	std::shared_ptr<EditorObjectGroup> GetOrPushGroup( const std::string& name)
	{
		auto result = TryGetObject<EditorObjectGroup>(name);
		if (!result.first)
			return PushObject<EditorObjectGroup>(tools.empty() ? 0: tools.rbegin()->first, name);

		return result.second;
	}




public:

	void DrawTool() override;
	void DrawHandleWithoutMenu();
	void DrawHandle() override;
};
