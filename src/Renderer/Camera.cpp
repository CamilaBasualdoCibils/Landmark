#include "pch.h"
#include "Camera.h"

#include "Entity/EntityManager.h"
#include "Transform/Transform.h"
#include "Renderer/Renderer.h"
Camera::Camera(EntityID id): IComponent(id)//,renderFBO(Renderer::GetRenderPass(),{800,800},{ColorFormat::RGBA8SRGB})
{

}
bool Camera::IsReady()
{
	if (transform.isValid()) return true;

	if (!EntityManager::HasComponent<Transform>(ID))
	{
		Enabled = false;
		logger.Error("Entity (" + std::to_string(ID) + " ) has no transform component. Disabling!");
		//logger << "Entiy (" << ID << ") has no transform component. Disabling!" << Log::end;
		return false;
	}

	transform = EntityManager::GetComponent<Transform>(ID);

	return true;



}

const mat4& Camera::GetViewMatrix()
{
	static glm::mat4 zero = glm::mat4(0);
	if (!Enabled || !IsReady()) return zero;
	
	if (transform->isDirty())
	{
		//TODO : IMPROVE THIS vvvvv
		viewMatrix = glm::inverse(transform->GetModelMatrix());
	}

	return viewMatrix;
}

const mat4& Camera::GetProjMatrix()
{
	if (projDirty)
	{
		mat4 mat;
		switch (projectionMode) {
		case ProjectionModes::PERSPECTIVE:
			mat = glm::perspective(GetFOVVertical(), AspectRatio, ZclipPlanes.x, ZclipPlanes.y);
			break;
		case ProjectionModes::INF_PERSPECTIVE:
			
			mat = infinitePerspective(GetFOVVertical(), AspectRatio,ZclipPlanes.x);
			break;
		case ProjectionModes::ORTHOGRAPHIC:
			mat = glm::identity<mat4>();
			logger.Critical("Implement this you fucking idiot");
			//logger.Critical() << "Implement this you fucking idiot" << Log::end;
			break;
		case ProjectionModes::INF_ORTHOGRAPHIC:
			mat = glm::identity<mat4>();
			logger.Critical("Implement this you fucking idiot");
			//logger.Critical() << "Implement this you fucking idiot" << Log::end;
			break;
		}
		projectionMatrix = mat;
		projDirty = false;
	}
	return projectionMatrix;
}
