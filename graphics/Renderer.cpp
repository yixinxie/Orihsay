#include "Renderer.h"
#include <string>
#include <unordered_map>
#include "../misc/CharHelper.h"
void Renderer::setMainCamera(const Vector3& pos, const Vector3& rot, const float fov, const float _nearPlane, const float _farPlane){
	CameraParameters* params = new CameraParameters();
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
void Renderer::updateInstancedObject(const int id, const Vector3& position, const Vector3& rotation, const Vector3& scale){
	
	ObjectInstanceTransform* oit = instancedObjects[id];
	if (oit == nullptr){
		TRACE("cannot find an instanced object with id"<<id);
	}
	oit->position = position;
	oit->rotation = rotation;
	oit->scale = scale;
}
int Renderer::registerInstancedObject(){
	int res;
	ObjectInstanceTransform* oit = new ObjectInstanceTransform();
	oit->position = Vector3(-1,-1,-1);
	oit->rotation = Vector3(-1, -1, -1);
	oit->scale = Vector3(-1, -1, -1);

	//instancedObjects.insert(std::make_pair<int, ObjectInstanceTransform*>(objectIndexIncrementer, oit));
	instancedObjects.insert({ objectIndexIncrementer, oit });
	res = objectIndexIncrementer;
	objectIndexIncrementer++;
	return res;
}