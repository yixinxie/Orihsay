#include "GameObject.h"

GameObject::GameObject(void){
	_transform = nullptr;
}
GameObject* GameObject::instantiate(int mode){
	GameObject* res = new GameObject();;
	if (mode == UseNoTransform){
	}
	else if (mode == UseTransform){
		res->_transform = new Transform();
		res->_transform->setGameObject(res);
	}
	else if (mode == UseRectTransform){
		res->_transform = new RectTransform();
		res->_transform->setGameObject(res);
	}
	else if (mode == HierarchyOnly){
		res->_transform = new BaseTransform();
		res->_transform->setGameObject(res);
	}
    
	return res;
}

void GameObject::setName(const char* _name){
	if (std::strlen(_name) <= 64)strcpy_s(name, _name);
}
void GameObject::awake(void){
	for (unsigned int i = 0; i < components.size(); i++){
		components.at(i)->awake();
	}
}
void GameObject::start(void){
	for (unsigned int i = 0; i < components.size(); i++){
		components.at(i)->start();
	}
}
void GameObject::update(void){
	for (unsigned int i = 0; i < components.size(); i++){
		//printf("mono %d start", i);
		components.at(i)->update();
		//printf("mono %d end", i);
	}
}
void GameObject::addComponent(MonoBehaviour* mono){
	mono->gameObject = this;
	components.push_back(mono);
}
void GameObject::onDestroy(){
	for (unsigned int i = 0; i < components.size(); i++){
		delete components.at(i);
	}
	delete _transform;
}