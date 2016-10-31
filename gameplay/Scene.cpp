#include <map>
#include "Scene.h"
#include "../misc/ClassFactory.h"
#include "../components/Camera.h"
#include "../misc/CharHelper.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "../misc/Macros.h"
using namespace rapidjson;

using namespace std;
Scene::Scene(void) :state(0){
	//deserialize();
	guidIncre = 1;
}
void Scene::update(void){
	if (state == 0){
		deserialize();
		for (unsigned int i = 0; i < gameObjects.size(); i++){
			gameObjects.at(i)->awake();
		}
		for (unsigned int i = 0; i < gameObjects.size(); i++){
			gameObjects.at(i)->start();
		}
		state = 1;
	}
	
	for (unsigned int i = 0; i < gameObjects.size(); i++){
		//printf("update go: %s start\n",  gameObjects.at(i)->name);
		gameObjects.at(i)->update();
		//printf("update go: %s ok\n", gameObjects.at(i)->name);
	}
}
void Scene::serialize(){

}

void Scene::deserialize(){
	CharBuffer* rawJSON = CharHelper::loadTextFile("scene0.json");
	Document parsed;
	parsed.Parse(rawJSON->buffer);
	bool d = parsed.HasParseError();
	ParseErrorCode code = parsed.GetParseError();
	
	for (unsigned int i = 0; i < parsed.Size(); i++){
		OriGUID guid;
		OriGUIDHelper::genGUID(&guid);
		printf_s("%lld,%lld\n", guid.d0, guid.d1);
		const Value& gameObjectNode = parsed[i];
		GameObject* go = nullptr;
		
		// transform type
		const char* typeStr = gameObjectNode["type"].GetString();
		if (strcmp(typeStr, "T") == 0){
			go = GameObject::instantiate(GameObject::UseTransform);
		}
		else if (strcmp(typeStr, "R") == 0){
			go = GameObject::instantiate(GameObject::UseRectTransform);
		}
		else if (strcmp(typeStr, "N") == 0){
			go = GameObject::instantiate(GameObject::UseNoTransform);
		}
		// guid
		{
			const char* tt = gameObjectNode["guid"].GetString();
			go->guid = CharHelper::charToInt(tt);
		}
		// by using the parent guid, locate the parent gameobject
		{
			const char* tt = gameObjectNode["parent"].GetString();
			int parentId = CharHelper::charToInt(tt);
			if (parentId >= 0){
				int arrayIndex = gameObjectIndexLookup.at(parentId);
				GameObject* parentGO = gameObjects.at(arrayIndex);
				go->parent = parentGO;
				parentGO->children.push_back(go);
			}
		}
		
		gameObjects.insert({ guidIncre, go });
		gameObjectIndexLookup.insert({ go->guid, guidIncre });
		guidIncre++;

		go->setName(gameObjectNode["name"].GetString());
		const Value& components = gameObjectNode["components"];
		// deserialize components
		for (unsigned int j = 0; j < components.Size(); j++){
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
				// so for the transform component we just set the values.
				const Value& fields = componentNode["fields"];
				const char* tmpPosition = fields["position"].GetString();
				go->transform()->position = CharHelper::charToVec3(tmpPosition);

				const char* tmpRotation = fields["rotation"].GetString();
				go->transform()->fromEuler(CharHelper::charToVec3(tmpRotation));

				const char* tmpScale = fields["scale"].GetString();
				go->transform()->scale = CharHelper::charToVec3(tmpScale);
				
			}
			else if (std::strcmp(componentNode["className"].GetString(), "RectTransform") == 0){
				const Value& fields = componentNode["fields"];
				const char* tmpPosition = fields["pivot"].GetString();
				go->rectTransform()->position = CharHelper::charToVec2(tmpPosition);
				

				const char* tmpSize = fields["anchorMin"].GetString();
				int encodedByte = 0;
				encodedByte = CharHelper::charToInt(tmpSize);
				go->rectTransform()->anchorMin = encodedByte;

				const char* tmp2 = fields["anchorMax"].GetString();
				encodedByte = CharHelper::charToInt(tmp2);
				go->rectTransform()->anchorMax = encodedByte;

				const char* tmp3 = fields["offsetMin"].GetString();
				go->rectTransform()->offsetMin = CharHelper::charToIntVec2(tmp3);

				const char* tmp4 = fields["offsetMax"].GetString();
				go->rectTransform()->offsetMax = CharHelper::charToIntVec2(tmp4);

				const char* tmp5 = fields["pivot"].GetString();
				go->rectTransform()->pivot = CharHelper::charToIntVec2(tmp5);
			}
			// for rest of the component types we need to add it to the game object.
			else if (std::strcmp(componentNode["className"].GetString(), "Cube") == 0){

				MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("CubeMesh");
				go->addComponent(component);
			}
			else if (std::strcmp(componentNode["className"].GetString(), "InputManager") == 0){

				MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("InputManager");
				go->addComponent(component);
			}
			else if (std::strcmp(componentNode["className"].GetString(), "Light") == 0){

				MonoBehaviour* component = (MonoBehaviour*)classFactory.construct("LightSource");
				go->addComponent(component);
			}
			else{
				const char* classNameStr = componentNode["className"].GetString();
				MonoBehaviour* component = (MonoBehaviour*)classFactory.construct(classNameStr);
				go->addComponent(component);

			}

		}
	}

	SAFE_DISPOSE(rawJSON);

}
void Scene::onDestroy(){
	for (unsigned int i = 0; i < gameObjects.size(); i++){
		gameObjects.at(i)->onDestroy();
		delete gameObjects.at(i);
	}

}
void Scene::newGameObjectTransform(char *objName, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8){
	GameObject* go = nullptr;
	go = GameObject::instantiate(GameObject::UseTransform);
	go->setName(objName);
	go->transform()->position = Vector3(f0, f1, f2);
	go->transform()->fromEuler(Vector3(f3, f4, f5));
	go->transform()->scale = Vector3(f6, f7, f8);
	
}
void Scene::newGameObjectRectTransform(char *objName, unsigned char anchorMin, unsigned char anchorMax, short offsetMinX, short offsetMinY, short offsetMaxX, short offsetMaxY, short pivotX, short pivotY){
	GameObject* go = nullptr;
	go = GameObject::instantiate(GameObject::UseRectTransform);
	go->setName(objName);
	//go->rectTransform()->position = Vector2(f0, f1);
	//go->rectTransform()->widthHeight = Vector2(f2, f3);
	
}