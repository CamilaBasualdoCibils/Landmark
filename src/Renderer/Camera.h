#pragma once
#include "Debug/Debug.h"
#include "Entity/IComponent.h"
//#include "VK/Rendering/Framebuffer.h"
class EntityManager;
#include "Transform/Transform.h"


struct Camera : public IComponent<Camera>
{
	enum class ProjectionModes {
		PERSPECTIVE,
		INF_PERSPECTIVE,
		ORTHOGRAPHIC,
		INF_ORTHOGRAPHIC
	};
private:
	static inline Log logger = Log("Camera");
	CompHandle<Transform> transform ;
	//Horizontal
	float _FOV = 70;

	vec2 ZclipPlanes = {0.1,100};
	float AspectRatio = 1;
	bool LogarithmicDepth = true;

	ProjectionModes projectionMode = ProjectionModes::PERSPECTIVE;

	bool projDirty = true;
	mat4 projectionMatrix;
	
	mat4 viewMatrix;

	//Framebuffer renderFBO;

public:
	Camera(EntityID id);
	~Camera()
	{
		//renderFBO.Destroy();
	}

private:
	bool IsReady();

public:

	constexpr float GetAspectRatio() const { return AspectRatio; }
	constexpr void SetAspectRatio(float a) { projDirty = true; AspectRatio = a; }

	constexpr float GetFOV() const { return _FOV; }
	constexpr void SetFOV(float f) { projDirty = true; _FOV = f; }
	float GetFOVVertical() const
	{
		return 2.0f * glm::atan(glm::tan(GetFOV() / 2.0f) / GetAspectRatio());
	}
	//const float GetFOV
	const mat4& GetViewMatrix();
	const mat4& GetProjMatrix();

	ProjectionModes GetProjectionMode() const { return projectionMode; }
	void SetProjectionMode(ProjectionModes pm) { projDirty = true; projectionMode = pm; }

	
};

