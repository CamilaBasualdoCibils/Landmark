#pragma once
#include <pch.h>

namespace Editor
{
bool DragVec3(const std::string &Label, vec3 *data,float v_speed = 1.0f);
bool Rotation(const std::string &Label, quat *data);
bool MenuItem(const std::string &Item, bool *selected, bool Enabled = true);

} // namespace Editor
