#pragma once
#include "EditorObject.hpp"
class EditorLambda : public EditorObject
{
	

	
	using GenericLambda_Func = std::function<void()>;
	GenericLambda_Func DrawHandle_Lambda;
	GenericLambda_Func DrawTool_Lambda;
	const static inline GenericLambda_Func defaultFunc = []() {};
public:
	EditorLambda(const std::string& name, GenericLambda_Func HandleFunc, GenericLambda_Func toolFunc = defaultFunc):EditorObject(name),DrawHandle_Lambda(HandleFunc),DrawTool_Lambda(toolFunc) {};
	void DrawTool() override { DrawTool_Lambda(); };
	void DrawHandle() override { DrawHandle_Lambda(); };
};
