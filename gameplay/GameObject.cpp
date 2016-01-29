#include "GameObject.h"
#include "Transform.h"
GameObject::GameObject(void){
	//transform.setGameObject(this);
	transform = new Transform();
	transform->setGameObject(this);
}
GameObject* GameObject::instantiate(){
	GameObject* res = nullptr;
	res = new GameObject();
	return res;
}
GameObject* GameObject::create(){
	GameObject* res = nullptr;
	res = new GameObject();
	return res;
}
void GameObject::setName(const char* _name){
	if (std::strlen(_name) <= 64)std::strcpy(name, _name);
}
void GameObject::awake(void){
	for (int i = 0; i < components.size(); i++){
		components.at(i)->awake();
	}
}
void GameObject::start(void){
	for (int i = 0; i < components.size(); i++){
		components.at(i)->start();
	}
}
void GameObject::update(void){
	for (int i = 0; i < components.size(); i++){
		components.at(i)->update();
	}
}
void GameObject::addComponent(MonoBehaviour* mono){
	mono->gameObject = this;
	components.push_back(mono);
}
void GameObject::onDestroy(){
	for (int i = 0; i < components.size(); i++){
		delete components.at(i);
	}
	delete transform;
}