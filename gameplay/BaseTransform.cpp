#include "BaseTransform.h"
#include "glm/gtx/rotate_vector.hpp"
#include "BaseTransform.h"
#include "GameObject.h"
void BaseTransform::c(){
	gameObject = nullptr;
	parent = nullptr;
	children.c();
}
void BaseTransform::d(){

	children.d();
}
void BaseTransform::setParent(BaseTransform* _transform){
	parent = _transform;
	if (_transform != nullptr)
		_transform->addChild(this);
}
void BaseTransform::addChild(BaseTransform* _transform){
	children.push(_transform);
}
//void BaseTransform::addChild(BaseTransform* _transform){
//	children.push_back(_transform);
//}
void BaseTransform::setGameObject(GameObject* _go){
	gameObject = _go;
}
GameObject* BaseTransform::getGameObject(void){
	return gameObject;
}