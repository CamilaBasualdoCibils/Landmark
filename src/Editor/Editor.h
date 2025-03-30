#pragma once
#include "EditorObjectGroup.h"
#include "imgui/imgui.h"
#include "EditorTopBar.hpp"
#include <Core/AppModule.h>
class Console;
class Editor : public AppModule
{

	//    Priority | toolset
	// std::map<>
	EditorTopBar topbar;

	static inline std::shared_ptr<Editor> INSTANCE;

public:
	Editor();
	static std::shared_ptr<Editor> GetInstance();
	void Init();
	void Draw();
	EditorTopBar &GetMainToolGroup() { return topbar; }
	std::vector<EngineCallInject> GetInjections() {
        std::vector<EngineCallInject> default_injects;
        EngineCallInject init_inj("Init");
		init_inj.priority = -10;
        init_inj.call = [this](){Init();};
        init_inj.call_point = EngineCallPoints::START;
        EngineCallInject exit_inj("Exit");
        exit_inj.call = [this](){Exit();};
        exit_inj.call_point = EngineCallPoints::END;
        return {init_inj,exit_inj};
    }
};
