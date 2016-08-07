#include "GameObject.h"

GameObject::GameObject(void){
	_transform = nullptr;
		 

}
GameObject* GameObject::instantiate(int mode){
	GameObject* res = nullptr;
	res = new GameObject();
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
		components.at(i)->update();
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