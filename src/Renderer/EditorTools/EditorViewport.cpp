#include "pch.h"
#include "EditorViewport.h"


#include "Renderer/Camera.h"
#include "Renderer/Renderer.hpp"

EditorViewport::EditorViewport(Renderer *_renderer) : EditorTool("Viewport"), renderer(_renderer)
{
	auto scene_frame = renderer->GetSceneFrame();
	auto &fbo = scene_frame->GetFramebuffer();
	auto &cis = fbo.GetImages()[0];
	scene_frame_ds = ImGui_ImplVulkan_AddTexture((vk::Sampler)cis.GetSampler(), (vk::ImageView)cis.GetImageView(), VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void EditorViewport::DrawHandle()
{
	ImGui::MenuItem("Viewport", 0, &ViewportOpen);
}

void EditorViewport::Draw()
{

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Cameras"))
		{
			/*
			const auto &entities = EntityManager::GetEntities();
			const auto &cameras = EntityManager::ComponentStorage<Camera>();
			for (auto &c : cameras)
			{
				std::string label = "Camera #" + std::to_string(c.ID) + " - " + entities[c.ID].Name;
				if (ImGui::MenuItem(label.c_str(), 0, CameraSelected == c.ID))
				{
					if (CameraSelected == c.ID)
					{
						CameraAssigned = false;
						CameraSelected = -1;
					}
					else
					{
						CameraSelected = c.ID;
						CameraAssigned = true;
					}
				}
			}*/

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (CameraAssigned)
	{
		// render camera

		return;
	}


	const auto available_region = ImGui::GetContentRegionAvail();
	const vec2 space = {available_region.x,available_region.y};
	last_viewport.size = space;
	const ImVec2 image_pos = ImGui::GetCursorPos();
	ImGui::Image((ImTextureID)(VkDescriptorSet)scene_frame_ds, available_region);
}

void EditorViewport::DrawTool()
{
	if (!ViewportOpen)
		return;
	if (ImGui::Begin(GetName().c_str(), &ViewportOpen, ImGuiWindowFlags_MenuBar))
		Draw();
	ImGui::End();
}

void EditorViewport::DrawNoCameraNotif() const
{
	const auto availableSpace = ImGui::GetContentRegionAvail();

	const auto WindowPos = ImGui::GetCursorScreenPos();
	const auto WindowSize = ImGui::GetContentRegionAvail();

	float LineLength = 0.6;
	float TextScale = 2;
	const char *const NoCameraText = "No Camera Assigned";
	auto textSize = ImGui::CalcTextSize(NoCameraText);
	textSize.x *= TextScale;
	textSize.y *= TextScale;
	// ImGui::GetWindowDrawList()->AddText({ WindowPos.x + WindowSize.x / 2 - textSize.x / 2,WindowPos.y + WindowSize.y / 2 - textSize.y / 2 }, UINT32_MAX, "No Camera Assigned");
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize() * TextScale, {WindowPos.x + WindowSize.x / 2 - textSize.x / 2, WindowPos.y + WindowSize.y / 2 - textSize.y / 2}, UINT32_MAX, NoCameraText);
	const glm::vec2 WindowMiddle = {WindowPos.x + WindowSize.x / 2, WindowPos.y + WindowSize.y / 2};
	for (int i = 0; i < 4; i++)
	{
		int x = (i & 0b1) ? 1 : -1;
		int y = (i & 0b10) ? 1 : -1;
		glm::vec2 Inner = WindowMiddle;
		glm::vec2 Outer = {WindowMiddle.x + (WindowSize.x * x) / 2, WindowMiddle.y + (WindowSize.y * y) / 2};

		glm::vec2 Mid = (Inner + Outer) / 2.f;

		Inner = glm::mix(Mid, Inner, LineLength);
		Outer = glm::mix(Mid, Outer, LineLength);

		ImGui::GetWindowDrawList()->AddLine({Inner.x, Inner.y}, {Outer.x, Outer.y}, UINT32_MAX, 1);
	}
	/*
	glm::vec2 min = { WindowPos.x  , WindowPos.y  };
	glm::vec2 max = min + glm::vec2{WindowSize.x, WindowSize.y};
	min = mix(WindowMiddle, min, LineLength);
	max = mix(WindowMiddle, max, LineLength);
	ImGui::GetWindowDrawList()->AddRect({ min.x,min.y }, { max.x,max.y }, UINT32_MAX, 5);
	*/
}

