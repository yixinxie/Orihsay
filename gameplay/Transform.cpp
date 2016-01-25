#include "Transform.h"

Transform::Transform(){
	/*position = { 0, 0, 0 };
	rotation = { 0, 0, 0 };
	scale = { 1, 1, 1 };*/
}
void Transform::setGameObject(GameObject* _gameObject){
	gameObject = _gameObject;
}