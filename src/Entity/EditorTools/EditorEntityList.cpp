#include "pch.h"
#include "EditorEntityList.h"

#include "Entity/EntityManager.h"
#include "imgui/imgui_internal.h"

EditorEntityList::EditorEntityList() :EditorTool("Entity List")
{

}

void EditorEntityList::DrawHandle()
{
	ImGui::MenuItem(GetName().c_str(), 0, &Open);
	ImGui::MenuItem(InspectorName.c_str(),0,&InspectorOpen);

}
static void DrawItemBackground(int iterator)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const auto oddColor = ImGui::ColorConvertFloat4ToU32({ 0.2,0.2,0.2,1 });
	const auto evenColor = ImGui::ColorConvertFloat4ToU32({ 0.1,0.1,0.1,1 });

	const ImGuiStyle& style = ImGui::GetStyle();
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
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
		ImGui::Button(" ", { 30,30 });

		ImGui::PopStyleVar();
		auto drawlist = ImGui::GetWindowDrawList();
		const glm::vec2 Max = { ImGui::GetItemRectMax().x,ImGui::GetItemRectMax().y };
		const glm::vec2 Min = { ImGui::GetItemRectMin().x,ImGui::GetItemRectMin().y };

		const glm::vec2 Size = Max - Min;
		const glm::vec2 mid = (Max + Min) / 2.0f;
		const float MaxSize = Size.x < Size.y ? Size.x : Size.y;
		const auto PlusColor = ImGui::ColorConvertFloat4ToU32({ 0.8,0.8,0.8,1 });
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
			drawlist->AddRectFilled({ rectMin.x,rectMin.y }, { rectMax.x,rectMax.y }, PlusColor, 3);
		}

	}

	ImGui::BeginChild("entities", { 0,0 }, true);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);
	draw_list->ChannelsSetCurrent(1);
	int counter = 0;



	const auto& entities = EntityManager::GetEntities();


	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		ImGui::PushID(it.GetIndex());

		if (ImGui::Selectable((*it).Name.c_str(), entitySelected == it.GetIndex())) entitySelected = it.GetIndex();
		DrawItemBackground(counter++);
		ImGui::PopID();

	}



	draw_list->ChannelsMerge();
	ImGui::EndChild();


}


void EditorEntityList::DrawTool()
{
	if (ImGui::Begin(GetName().c_str(),&Open))
	{
		Draw();

	}
	ImGui::End();

	if (ImGui::Begin(InspectorName.c_str())) {
		DrawInspector();
	}
	ImGui::End();
}

void EditorEntityList::DrawInspector()
{
	if (EntityManager::ExistsEntity(entitySelected)) {
		const auto entity = EntityManager::GetEntity(entitySelected);

		ImGui::BeginChild(ImGuiID(1),{0,0},true);
		int i = 0;
		for (const ComponentTypeID& comp_id: entity->Components){
			const auto desc = ComponentRegistry::GetComponentDesc(comp_id);
			ImGui::PushID(i);
			if (ImGui::CollapsingHeader(desc.Name.c_str(),ImGuiTreeNodeFlags_Framed)) {
				IComponentData* comp = EntityManager::GetComponent(entitySelected,comp_id);
			ComponentRegistry::GetComponentDesc(comp_id).inspectorFunction(comp);
			}
			ImGui::PopID();
			i++;
		}

		ImGui::EndChild();
	}
}
