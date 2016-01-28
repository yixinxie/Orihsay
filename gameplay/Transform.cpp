#include "Transform.h"

Transform::Transform(){
	
	position = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
}
void Transform::setGameObject(GameObject* _gameObject){
	gameObject = _gameObject;
}