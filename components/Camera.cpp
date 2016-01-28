#include "Camera.h"
#include "../graphics/Renderer.h"
#include "../gameplay/GameObject.h"
#include "../gameplay/G.h"

void Camera::awake(){
	if (_isMain){
		//Vector3 p = gameObject->transform->position;
		G::instance()->renderer->setMainCamera(gameObject->transform->position, gameObject->transform->rotation, fieldOfView, nearPlane, farPlane);
	}
}
