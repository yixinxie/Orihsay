#include "Camera.h"
#include "../graphics/Renderer.h"
#include "../gameplay/GameObject.h"
#include "../gameplay/G.h"

void Camera::awake(){
	if (_isMain){
		//Vector3 p = gameObject->transform->position;
		G::instance()->renderer->setMainCamera(gameObject->transform()->position, gameObject->transform()->getEuler(), fieldOfView, nearPlane, farPlane);
	}
}

void Camera::update(){
	G::instance()->renderer->updateMainCamera(gameObject->transform()->position, gameObject->transform()->getEuler());
}