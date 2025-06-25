#pragma once
#include "Editor/Debug.h"
//#include "VK/Rendering/Framebuffer.h"
class EntityManager;
#include "Transform/Transform.h"


struct Camera 
{
	enum class ProjectionModes {
		PERSPECTIVE,
		INF_PERSPECTIVE,
		ORTHOGRAPHIC,
		INF_ORTHOGRAPHIC
	};
	
	protected:
	static inline Log logger = Log("Camera");
	
	//Horizontal
	float _FOV = 70;
	float _SensorSize = 1;
	vec2 ZclipPlanes = {0.1,100};
	float AspectRatio = 1;
	bool LogarithmicDepth = true;

	ProjectionModes projectionMode = ProjectionModes::PERSPECTIVE;

	bool projDirty = true;
	mat4 projectionMatrix;
	
	mat4 viewMatrix;
	mat4 last_model_matrix;

	//Framebuffer renderFBO;

public:
	Camera();
	~Camera()
	{
		//renderFBO.Destroy();
	}

private:


public:

	constexpr float GetAspectRatio() const { return AspectRatio; }
	constexpr void SetAspectRatio(float a) { projDirty = true; AspectRatio = a; }

	constexpr float GetFOV() const { return _FOV; }
	constexpr void SetFOV(float f) { projDirty = true; _FOV = f; }
	constexpr void SetSensorSize(float f) {projDirty = true;_SensorSize = f;}
	constexpr vec2 GetZPlanes() const { return ZclipPlanes;}
	constexpr void SetZPlanes(vec2 pl) {projDirty = true; ZclipPlanes = pl;}
	float GetFOVVertical() const
	{
		return 2.0f * glm::atan(glm::tan(glm::radians( GetFOV()) / 2.0f) / GetAspectRatio());
	}
	//const float GetFOV
	const mat4& GetViewMatrix(Transform& t);
	const mat4& GetProjMatrix();

	ProjectionModes GetProjectionMode() const { return projectionMode; }
	void SetProjectionMode(ProjectionModes pm) { projDirty = true; projectionMode = pm; }

	
};

