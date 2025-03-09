#include "pch.h"
#include "Camera.h"

#include "Transform/Transform.h"
//#include "Renderer/Renderer.h"
Camera::Camera()//,renderFBO(Renderer::GetRenderPass(),{800,800},{ColorFormat::RGBA8SRGB})
{

}


const mat4& Camera::GetViewMatrix(Transform& transform)
{
	static glm::mat4 zero = glm::mat4(0);
	
	if (transform.isDirty())
	{
		//TODO : IMPROVE THIS vvvvv
		viewMatrix = glm::inverse(transform.GetModelMatrix());
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
