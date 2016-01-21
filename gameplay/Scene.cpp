#include <map>
#include "Scene.h"
#include "../misc/ClassFactory.h"
using namespace std;
Scene::Scene(void){
			
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
	MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("Panel");
	go->addComponent(component);
	
}
