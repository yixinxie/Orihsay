#include "GameObject.h"
GameObject::GameObject(void){

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
	components.push_back(mono);
}