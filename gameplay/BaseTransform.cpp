#include "BaseTransform.h"
#include "glm/gtx/rotate_vector.hpp"
#include "BaseTransform.h"
BaseTransform::BaseTransform(){
	gameObject = nullptr;
	parent = nullptr;
}
void BaseTransform::setParent(GameObject* _go){
	parent = _go;
}
void BaseTransform::addChild(GameObject* _go){
	children.push_back(_go);
}
void BaseTransform::setGameObject(GameObject* _go){
	gameObject = _go;
}
GameObject* BaseTransform::getGameObject(void){
	return gameObject;
}