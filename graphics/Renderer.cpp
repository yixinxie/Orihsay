#include "Renderer.h"

void Renderer::setMainCamera(Vector3& pos, Vector3& rot, float fov, float _nearPlane, float _farPlane){
	CameraParameters* params = new CameraParameters();
	params->position = pos;
	params->rotation = rot;
	params->fieldOfView = fov;
	params->nearPlane = _nearPlane;
	params->farPlane = _farPlane;
	
	cameras.push_back(params);
}