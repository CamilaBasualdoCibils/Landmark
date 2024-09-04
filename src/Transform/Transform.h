#pragma once
#include "Entity/IComponent.h"
struct Transform final : public IComponent<Transform>
{
private:

	vec3 Position = {0,0,0};
	quat Rotation = quat({0,0,0});
	vec3 Scale = {1,1,1};

	bool ModelMatrixDirty = true;
	mat4 ModelMatrix;
public:
	explicit Transform(EntityID id)
		: IComponent(id)
	{
	}

	const mat4& GetModelMatrix()
	{
		if (ModelMatrixDirty)
		{
			mat4 T = translate(identity<mat4>(), Position);
			mat4 R(Rotation);
			mat4 S = scale(identity<mat4>(), Scale);
			ModelMatrix = T * R * S;
			ModelMatrixDirty = false;
		}
		return ModelMatrix;
		
	}

	constexpr bool isDirty() const { return ModelMatrixDirty; }

	static void ComponentInspector(IComponentData* comp) {
		Transform* trans = reinterpret_cast<Transform*>(comp);
		bool tModified = ImGui::DragFloat3("Position",&trans->Position[0]);
		vec3 euler = glm::eulerAngles(trans->Rotation);
		
		bool rModified = ImGui::DragFloat3("Rotation",&euler.x);
		if (rModified) trans->Rotation = quat(euler);
		bool sModified = ImGui::DragFloat3("Scale",&trans->Scale[0]);

		trans->ModelMatrixDirty = trans->ModelMatrixDirty || tModified || rModified || sModified;
	}
	
};