#include "InputManager.h"
#include "../gameplay/G.h"
#include "../misc/CharHelper.h"
#include "../gameplay/GameObject.h"
void InputManager::awake(void){

}
void InputManager::update(void){
	
	if (G::instance()->input->getMouseButton(0) == true){
		// pan the camera
		IntVector2 thisMousePos = G::instance()->input->mousePosition();
		gameObject->transform->rotation.z += (thisMousePos.x - lastMousePos.x) *0.1f;
		gameObject->transform->rotation.y += (thisMousePos.y - lastMousePos.y) *0.1f;
		
		if (gameObject->transform->rotation.y > 3.14156f / 2){
			gameObject->transform->rotation.y = 3.14156f / 2;
		}
		else if (gameObject->transform->rotation.y < -3.14156f / 2){
			gameObject->transform->rotation.y = -3.1415f / 2;
		}
		lastMousePos = thisMousePos;

	}
	if (G::instance()->input->getKeyDown(KeyCode::W)){
		gameObject->transform->position.z += 0.1f;
	}
	if (G::instance()->input->getKeyDown(KeyCode::S)){
		gameObject->transform->position.z -= 0.1f;
	}
	if (G::instance()->input->getKeyDown(KeyCode::A)){
		gameObject->transform->position.x += 0.1f;
	}
	if (G::instance()->input->getKeyDown(KeyCode::D)){
		gameObject->transform->position.x -= 0.1f;
	}
	
}
