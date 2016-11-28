#include "GameObject.h"
#include "../misc/BasicMem.h"
void GameObject::c(void){
	_transform = nullptr;
	components.c();
}
void GameObject::d(){

}
GameObject* GameObject::instantiate(int mode){
	GameObject* res = ori_alloc(GameObject);
	res->c();
	if (mode == UseNoTransform){
	}
	else if (mode == UseTransform){
		res->_transform = ori_alloc(Transform);
		res->transform()->c();
		res->transform()->setGameObject(res);
	}
	else if (mode == UseRectTransform){
		res->_transform = ori_alloc(RectTransform);
		res->rectTransform()->c();
		res->rectTransform()->setGameObject(res);
	}
	else if (mode == HierarchyOnly){
		res->_transform = ori_alloc(BaseTransform);
		res->_transform->c();
		res->_transform->setGameObject(res);
	}
    
	return res;
}

void GameObject::setName(const char* _name){
	if (std::strlen(_name) <= 64)strcpy_s(name, _name);
}
void GameObject::awake(void){
	for (unsigned int i = 0; i < components.length; i++){
		((MonoBehaviour*)components[i])->awake();
	}
}
void GameObject::start(void){
	for (unsigned int i = 0; i < components.length; i++){
		((MonoBehaviour*)components[i])->start();
	}
}
void GameObject::update(void){
	for (unsigned int i = 0; i < components.length; i++){
		//printf("mono %d start", i);
		((MonoBehaviour*)components[i])->update();
		//printf("mono %d end", i);
	}
}
void GameObject::addComponent(MonoBehaviour* mono){
	mono->gameObject = this;
	components.push(mono);
}
void GameObject::onDestroy(){
	for (unsigned int i = 0; i < components.length; i++){
		ori_dealloc(components[i]);
		//delete components.at(i);
	}
	//delete _transform;
	ori_dealloc(_transform);
}