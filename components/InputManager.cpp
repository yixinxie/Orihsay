#include "InputManager.h"
#include "windows.h"
#include "../gameplay/G.h"
#include "../misc/CharHelper.h"
#include "../gameplay/GameObject.h"
void InputManager::awake(void){
	leftButtonDown = false;
}
void InputManager::update(void){
	
	if (G::instance()->input->getMouseButton(0) == true){
		// pan the camera
		IntVector2 thisMousePos = G::instance()->input->mousePosition();
		if (leftButtonDown == false){
			leftButtonDown = true;
		}
		else{
			if (thisMousePos.x - lastMousePos.x != 0){
				int d = 0;
			}
			//gameObject->transform->rotate(Vector3(0, 1, 0), (float)(thisMousePos.x - lastMousePos.x) *0.01f);
			gameObject->transform->rotate(Vector3(0, 1, 0), (float)(thisMousePos.x) *0.06f);
			gameObject->transform->rotate(gameObject->transform->getRight(), (float)(thisMousePos.y) *0.06f);
		}
		lastMousePos = thisMousePos;

	}
	else{
		leftButtonDown = false;

	}
	//if ((GetAsyncKeyState(0x57) & 0x8000)>0)
	if (G::instance()->input->getKeyDown(KeyCode::W))
	{
		gameObject->transform->position += gameObject->transform->getLookat() * 0.1f;

	}
	if (G::instance()->input->getKeyDown(KeyCode::S)){
		gameObject->transform->position -= gameObject->transform->getLookat() * 0.1f;

	}
	if (G::instance()->input->getKeyDown(KeyCode::A)){
		gameObject->transform->position -= gameObject->transform->getRight() * 0.1f;

	}
	if (G::instance()->input->getKeyDown(KeyCode::D)){
		gameObject->transform->position += gameObject->transform->getRight() * 0.1f;

	}
	
}
