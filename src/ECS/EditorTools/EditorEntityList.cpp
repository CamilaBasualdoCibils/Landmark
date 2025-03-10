#include "pch.h"
#include "EditorEntityList.h"

#include <ECS/ECS.hpp>
#include "imgui/imgui_internal.h"
#include <Core/App.h>
#include <ECS/Base/EntityData.hpp>
#include <ECS/Base/ComponentData.hpp>
EditorEntityList::EditorEntityList() : EditorTool("Entity List")
{
	scene_manager = App::GetInstance()->GetModule<SceneManager>();
}

void EditorEntityList::DrawHandle()
{
	ImGui::MenuItem(GetName().c_str(), 0, &Open);
	ImGui::MenuItem(InspectorName.c_str(), 0, &InspectorOpen);
}
static void DrawItemBackground(int iterator)
{
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	const auto oddColor = ImGui::ColorConvertFloat4ToU32({0.2, 0.2, 0.2, 1});
	const auto evenColor = ImGui::ColorConvertFloat4ToU32({0.1, 0.1, 0.1, 1});

	const ImGuiStyle &style = ImGui::GetStyle();
	auto itemMin = ImGui::GetItemRectMin(),
		 itemMax = ImGui::GetItemRectMax(),
		 itemSize = ImGui::GetItemRectSize(),
		 contentmin = ImGui::GetWindowContentRegionMin(),
		 contentMax = ImGui::GetContentRegionMax();

	float xSize = ImGui::GetContentRegionAvail().x;

	itemMin.x = ImGui::GetWindowPos().x;
	itemMin.y -= style.ItemSpacing.y / 2.f;

	itemMax.y += style.ItemSpacing.y / 2.f;
	itemMax.x = contentMax.x + ImGui::GetWindowPos().x;
	draw_list->ChannelsSetCurrent(0);

	draw_list->AddRectFilled(itemMin, itemMax, iterator & 0x1 ? oddColor : evenColor);

	draw_list->ChannelsSetCurrent(1);
}
void EditorEntityList::Draw()
{

	{
		// ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
		bool create_entity_pressed = ImGui::Button(" ", {30, 30});
		if (create_entity_pressed)
		{
			ImGui::OpenPopup(entity_create_popup_name.c_str());
		}
		if (ImGui::BeginPopup(entity_create_popup_name.c_str()))
		{
			ImGui::SeparatorText("properties");

			ImGui::EndPopup();
		}
		// ImGui::PopStyleVar();
		auto drawlist = ImGui::GetWindowDrawList();
		const glm::vec2 Max = {ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y};
		const glm::vec2 Min = {ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y};

		const glm::vec2 Size = Max - Min;
		const glm::vec2 mid = (Max + Min) / 2.0f;
		const float MaxSize = Size.x < Size.y ? Size.x : Size.y;
		const auto PlusColor = ImGui::ColorConvertFloat4ToU32({0.8, 0.8, 0.8, 1});
		const float SizeMultiplier = 0.8;
		const float LessSideRatio = 0.3;

		for (int i = 0; i < 2; i++)
		{
			bool Upwards = i == 0;
			glm::vec2 rectMin = mid, rectMax = mid;
			const float LongLength = (MaxSize / 2) * SizeMultiplier;
			rectMin[!Upwards] += -(LongLength * LessSideRatio);
			rectMin[Upwards] += -LongLength;
			rectMax[!Upwards] += (LongLength * LessSideRatio);
			rectMax[Upwards] += LongLength;
			drawlist->AddRectFilled({rectMin.x, rectMin.y}, {rectMax.x, rectMax.y}, PlusColor, 3);
		}
	}

	ImGui::BeginChild("entities", {0, 0}, true);

	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);
	draw_list->ChannelsSetCurrent(1);
	int counter = 0;

	auto &scenes = scene_manager->GetScenes();
	for (auto s_it = scenes.begin(); s_it != scenes.end(); ++s_it)
	{
		Scene &scene = *(s_it->second);
		ImGui::PushID(s_it->first);
		std::string Full_Name = "Scene::" + scene.GetName();
		ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
		if (ImGui::TreeNode(Full_Name.c_str()))
		{

			auto &entities = scene.GetEntities();
			for (auto e_it = entities.begin(); e_it != entities.end(); ++e_it)
			{
				EntityData &data = *e_it.Get<EntityData>();
				ImGui::PushID(e_it.GetIndex());
				// ImGui::Text(data.GetName().data());
				if (ImGui::Selectable(data.GetName().data(), entitySelected.isValid() && entitySelected->ID == e_it.GetIndex()))
					entitySelected = Entity(s_it->first, e_it.GetIndex());
				DrawItemBackground(counter++);
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	/*

		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			ImGui::PushID(it.GetIndex());

			if (ImGui::Selectable((*it).Name.c_str(), entitySelected == it.GetIndex())) entitySelected = it.GetIndex();
			DrawItemBackground(counter++);
			ImGui::PopID();

		}*/

	draw_list->ChannelsMerge();
	ImGui::EndChild();
}

void EditorEntityList::DrawTool()
{
	if (ImGui::Begin(GetName().c_str(), &Open))
	{
		Draw();
	}
	ImGui::End();

	if (ImGui::Begin(InspectorName.c_str()))
	{
		DrawInspector();
	}
	ImGui::End();
}

void EditorEntityList::DrawInspector()
{

	if (entitySelected.isValid())
	{
		ImGui::BeginChild(ImGuiID(1), {0, 0}, true);
		int i = 0;
		const auto &GetOwningScene = scene_manager->GetScene(entitySelected->scene_id);
		for (const ComponentTypeID &comp_id : entitySelected->GetComponents())
		{
			if (comp_id == 0)
				continue;

			ImGui::PushID(i);
			const auto &comp_info = ComponentRegistry::GetComponentInfo(comp_id);
			ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
			if (ImGui::TreeNode(comp_info.Name.c_str()))
			{
				IComponentData *comp = GetOwningScene->GetComponent(entitySelected->ID, comp_id);

				comp->DrawInspector();

				// GetOwningScene
				// const auto desc = ComponentRegistry::GetComponentDesc(comp_id);
				// if (ImGui::CollapsingHeader(desc.Name.c_str(),ImGuiTreeNodeFlags_Framed)) {
				//	IComponentDataData* comp = EntityManager::GetComponent(entitySelected,comp_id);
				// ComponentRegistry::GetComponentDesc(comp_id).inspectorFunction(comp);
				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}

		float button_x_size = ImGui::GetContentRegionAvail().x;
		if (ImGui::Button("Add Component", {button_x_size, 0}))
		{
			ImGui::OpenPopup(comp_create_popup_name.c_str());
		}
		ComponentTypeID selected_comp = 0;
		if (ImGui::BeginPopup(comp_create_popup_name.c_str()))
		{
			ImGui::BeginChild("PopupChild", ImVec2(200, 150), true); // Scrollable area

			const auto& registered_components = ComponentRegistry::GetRegisteredComponents();
			for (int i = 1; i < registered_components.size();i++)
			{
				if (entitySelected->Has_Component(i)) continue;
				const auto& comp_type = registered_components[i];
				if (ImGui::Selectable(comp_type.Name.c_str()))
				{
					selected_comp = static_cast<int>(i);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndChild();

			ImGui::EndPopup();
		}
		ImGui::EndChild();

		if (selected_comp != 0) {
			entitySelected->GetScene()->AddComponent(entitySelected.GetObjectID(),selected_comp);
		}
	}
	/*
	if (EntityManager::ExistsEntity(entitySelected)) {
		const auto entity = EntityManager::GetEntity(entitySelected);

		ImGui::BeginChild(ImGuiID(1),{0,0},true);
		int i = 0;
		for (const ComponentTypeID& comp_id: entity->Components){
			const auto desc = ComponentRegistry::GetComponentDesc(comp_id);
			ImGui::PushID(i);
			if (ImGui::CollapsingHeader(desc.Name.c_str(),ImGuiTreeNodeFlags_Framed)) {
				IComponentDataData* comp = EntityManager::GetComponent(entitySelected,comp_id);
			ComponentRegistry::GetComponentDesc(comp_id).inspectorFunction(comp);
			}
			ImGui::PopID();
			i++;
		}

		ImGui::EndChild();
	}*/
}
