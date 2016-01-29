#include <map>
#include "Scene.h"
#include "../misc/ClassFactory.h"
#include "../components/Camera.h"
#include "../misc/CharHelper.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../misc/Macros.h"
using namespace rapidjson;

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
//void Scene::deserialize(){
//	GameObject* go = GameObject::instantiate();
//	gameObjects.push_back(go);
//	MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("Camera");
//	go->addComponent(component);
//	((Camera*)component)->nearPlane = 0.01f;
//	((Camera*)component)->farPlane = 1000;
//	((Camera*)component)->fieldOfView = 60;
//	((Camera*)component)->gameObject->transform->position = Vector3(0, 0, -1);
//	((Camera*)component)->gameObject->transform->rotation = Vector3(0, 0, 0);
//	((Camera*)component)->gameObject->transform->scale = Vector3(1, 1, 1);
//	
//}


void Scene::deserialize(){
	CharBuffer* rawJSON = CharHelper::loadTextFile("scene0.json");

	Document parsed;
	parsed.Parse(rawJSON->buffer);
	bool d = parsed.HasParseError();
	ParseErrorCode code = parsed.GetParseError();
	
	for (int i = 0; i < parsed.Size(); i++){
		const Value& gameObjectNode = parsed[i];
		GameObject* go = GameObject::instantiate();
		gameObjects.push_back(go);
		go->setName(gameObjectNode["name"].GetString());
		const Value& components = gameObjectNode["components"];
		// deserialize components
		for (int j = 0; j < components.Size(); j++){
			const Value& componentNode = components[j];
			const char* dbgstr = componentNode["className"].GetString();
			if (std::strcmp(componentNode["className"].GetString(), "Camera") == 0){
				MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("Camera");
				go->addComponent(component);
				const Value& fields = componentNode["fields"];
				((Camera*)component)->nearPlane = CharHelper::charToFloat(fields["nearPlane"].GetString());
				((Camera*)component)->farPlane = CharHelper::charToFloat(fields["farPlane"].GetString());
				((Camera*)component)->fieldOfView = CharHelper::charToFloat(fields["fieldOfView"].GetString());

			}
			else if (std::strcmp(componentNode["className"].GetString(), "Transform") == 0){
				if (componentNode.HasMember("fields") == false){
					int sd = 0;
				}
				const Value& fields = componentNode["fields"];
				const char* tmpPosition = fields["position"].GetString();
				go->transform->position = CharHelper::charToVec3(tmpPosition);

				const char* tmpRotation = fields["rotation"].GetString();
				go->transform->rotation = CharHelper::charToVec3(tmpRotation);

				const char* tmpScale = fields["scale"].GetString();
				go->transform->scale = CharHelper::charToVec3(tmpScale);
				
				int dsdf = 0;
			}
		}

	}


	SAFE_DISPOSE(rawJSON);

}
void Scene::onDestroy(){
	for (int i = 0; i < gameObjects.size(); i++){
		gameObjects.at(i)->onDestroy();
		delete gameObjects.at(i);
	}

}