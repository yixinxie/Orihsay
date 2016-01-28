#include <map>
#include "Scene.h"
#include "../misc/ClassFactory.h"
#include "../components/Camera.h"
using namespace std;
Scene::Scene(void) :state(0){
	//deserialize();
}
void Scene::update(void){
	if (state == 0){
		deserialize();

		for (int i = 0; i < gameObjects.size(); i++){
			gameObjects.at(i)->awake();
		}

		for (int i = 0; i < gameObjects.size(); i++){
			gameObjects.at(i)->start();
		}
		state = 1;
	}
	for (int i = 0; i < gameObjects.size(); i++){
		gameObjects.at(i)->update();
	}
}
void Scene::serialize(){

}
void Scene::deserialize(){
	GameObject* go = GameObject::instantiate();
	gameObjects.push_back(go);
	MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("Camera");
	go->addComponent(component);
	((Camera*)component)->nearPlane = 0.01f;
	((Camera*)component)->farPlane = 1000;
	((Camera*)component)->fieldOfView = 60;
	((Camera*)component)->gameObject->transform->position = Vector3(0, 0, -1);
	((Camera*)component)->gameObject->transform->rotation = Vector3(0, 0, 0);
	((Camera*)component)->gameObject->transform->scale = Vector3(1, 1, 1);
	
}
void Scene::onDestroy(){
	for (int i = 0; i < gameObjects.size(); i++){
		gameObjects.at(i)->onDestroy();
		delete gameObjects.at(i);
	}

}