#include "SceneManager.h"
SceneManager::SceneManager(void){
	currentSceneIndex = 0;
	Scene* testScene = new Scene();
	scenes.push_back(testScene);
}
void SceneManager::update(void){
	scenes.at(currentSceneIndex)->update();
}
void SceneManager::onDestroy(){
	for (int i = 0; i < scenes.size(); i++){
		scenes.at(i)->onDestroy();
		delete scenes.at(i);
	}

}