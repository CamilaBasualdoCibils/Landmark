#include "pch.h"
#include "Editor.h"

#include "EditorLambda.h"
#include "Logo.h"
#include "EditorIcons.hpp"
#include <Core/App.h>
#include "Console/Console.hpp"
Editor::Editor()
{

}
 std::shared_ptr<Editor> Editor::GetInstance()
{
	if (!INSTANCE) INSTANCE = App::GetInstance()->GetModule<Editor>();
    return INSTANCE;
}
void Editor::Init()
{

	topbar.GetViewGroup()->PushObject<EditorLambda>(INT64_MIN, "Font Scale", []()
												   {
			float& scale = ImGui::GetIO().FontGlobalScale;
			if (ImGui::InputFloat("Font Scale", &scale, 0.1, 0.25))
			{
				scale = std::max(std::min(scale, 5.0f), 0.25f);
				//ImGui::GetStyle().ScaleAllSizes(scale / Previous);
			} });
	topbar.GetPanelsGroup()->PushObject<Console>(0);
	Asset asset("assets/editor/icons/EditorUI.png");
	EditorIcons::LoadIconSheet(asset);
}

void Editor::Draw()
{
	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGui::Begin("Editor", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	{
		const auto size = ImGui::GetWindowContentRegionMax();
		const auto pos = ImGui::GetWindowPos();
		const auto textsize = ImGui::CalcTextSize(LandmarkLogo.c_str());
		const float brightness = 0.35;

		const auto LogoColor = ImGui::ColorConvertFloat4ToU32({brightness, brightness, brightness, 1});
		const auto backgroundColor = ImGui::ColorConvertFloat4ToU32({8 / 256.f, 8 / 256.f, 10 / 256.f, 1});
		ImGui::GetWindowDrawList()->AddRectFilled({pos.x, pos.y}, {pos.x + size.x, pos.y + size.y}, backgroundColor);
		ImGui::GetWindowDrawList()->AddRect({pos.x + size.x / 2 - textsize.x / 2, pos.y + size.y / 2 - textsize.y / 2}, {pos.x + size.x / 2 + textsize.x / 2, pos.y + size.y / 2 + textsize.y / 2}, LogoColor, 6, 0, 1);
		ImGui::GetWindowDrawList()->AddText({pos.x + size.x / 2 - textsize.x / 2, pos.y + size.y / 2 - textsize.y / 2}, LogoColor, LandmarkLogo.c_str());
	}

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");

	// ImGui::PushStyleVar(ImGuiStyleVar_Alpha, );
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	// ImGui::PopStyleVar();

	if (ImGui::BeginMenuBar())
	{

		topbar.DrawHandleWithoutMenu();
		ImGui::EndMenuBar();
	}
	ImGui::End();

	topbar.DrawTool();
}
