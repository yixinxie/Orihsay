#include "Camera.h"
#include "../graphics/Renderer.h"
#include "../gameplay/GameObject.h"
void Camera::awake(){
	if (_isMain){
		G::instance()->renderer->setMainCameraTransform(gameObject->transform);
	}
}
