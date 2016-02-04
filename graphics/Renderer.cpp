#include "Renderer.h"
#include <string>
#include <unordered_map>
#include "../misc/CharHelper.h"

using namespace OriGraphics;
void Renderer::setMainCamera(const Vector3& pos, const Vector3& rot, const float fov, const float _nearPlane, const float _farPlane){
	CameraDesc* params = new CameraDesc();
	params->position = pos;
	params->rotation = rot;
	params->fieldOfView = fov;
	params->nearPlane = _nearPlane;
	params->farPlane = _farPlane;
	
	cameras.push_back(params);
}
void Renderer::updateMainCamera(const Vector3& pos, const Vector3& rot){
	if (cameras.size() == 0 || cameras[0] == nullptr)return;
	cameras[0]->position = pos;
	cameras[0]->rotation = rot;

}
int Renderer::registerInstancedObject(){
	int res;
	ObjectTransformDesc* oit = new ObjectTransformDesc();
	oit->position = Vector3(-1, -1, -1);
	oit->rotation = Vector3(-1, -1, -1);
	oit->scale = Vector3(-1, -1, -1);

	instancedObjects.insert({ objectIndexIncrementer, oit });
	res = objectIndexIncrementer;
	objectIndexIncrementer++;
	return res;
}
void Renderer::updateInstancedObject(const int id, const Vector3& position, const Vector3& rotation, const Vector3& scale){
	
	ObjectTransformDesc* oit = instancedObjects[id];
	if (oit == nullptr){
		TRACE("cannot find an instanced object with id"<<id);
		return;
	}
	oit->position = position;
	oit->rotation = rotation;
	oit->scale = scale;
}

int Renderer::registerLightSource(){
	int res;
	LightSourceDesc* lightSourceDesc = new LightSourceDesc();

	lightSources.insert({ lightIndexIncrementer, lightSourceDesc});
	res = lightIndexIncrementer;
	lightIndexIncrementer++;
	return res;
}
void Renderer::updateLightSource(const int id, const Vector3& position, const Vector3& rotation){

	LightSourceDesc* lightSourceDesc = lightSources[id];
	if (lightSourceDesc == nullptr){
		TRACE("cannot find the light source with id" << id);
		return;
	}
	lightSourceDesc->position = position;
	lightSourceDesc->rotation = rotation;
}