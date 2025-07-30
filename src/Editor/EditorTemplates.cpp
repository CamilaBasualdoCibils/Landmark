#include "EditorTemplates.hpp"

bool Editor::DragVec3(const std::string &Label, vec3 *data,float v_speed)
{
    return ImGui::DragFloat3(Label.c_str(), &data->x,v_speed);
}

bool Editor::Rotation(const std::string &Label, quat *data)
{
    vec3 euler = glm::degrees(glm::eulerAngles(*data));
    bool a = ImGui::DragFloat4(Label.c_str(), &data->x);
    bool b = ImGui::DragFloat3((Label + " Euler").c_str(),&euler.x);
    if (b) *data = quat(glm::radians(euler));
    return a || b;
}

bool Editor::MenuItem(const std::string &Item, bool *selected, bool Enabled)
{
    return ImGui::MenuItem(Item.c_str(), nullptr, selected, Enabled);
}


