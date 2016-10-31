#include "Renderer.h"
#include <string>
#include <unordered_map>
#include "../misc/CharHelper.h"

using namespace OriGraphics;
void Renderer::setMainCamera(const Vector3& pos, const Vector3& rot, const float fov, const float _nearPlane, const float _farPlane){
	CameraDesc* params = new CameraDesc();
	//params->position = pos;
	//params->rotation = rot;
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
int Renderer::registerSpriteObject(){
	int res;
	ObjectRectTransformDesc* desc = new ObjectRectTransformDesc();
	desc->position = Vector2(0, 0);
	desc->dirty = 1;
	spriteObjects.insert({ spriteIndexIncrementer, desc });
	res = spriteIndexIncrementer;
	spriteIndexIncrementer++;
	return res;
}
void Renderer::updateSpriteObject(const int id, const Vector2& position, const Vector2& widthHeight){

	ObjectRectTransformDesc* desc = spriteObjects[id];
	if (desc == nullptr){
		TRACE("cannot find a sprite object with id" << id);
		return;
	}
	desc->dirty = 1;
	desc->position = position;
	desc->widthHeight = widthHeight;
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
void Renderer::updateRectTransforms(int idx, int parentLeft, int parentBottom, int parentTop, int parentRight){
	for (int i = 0; i < spriteObjects.size(); i++){
		ObjectRectTransformDesc* desc = spriteObjects.at(i);
		if (desc->dirty != 0 && desc->children != nullptr){
			int parentWidthHalf = (parentRight - parentLeft) / 2;
			int parentHeightHalf = (parentTop - parentBottom) / 2;
			
			int thisLeft, thisBottom, thisRight, thisTop;
			int minX = (desc->anchorMin & 0x3);
			thisLeft = parentLeft + parentWidthHalf * minX + desc->offsetMin.x;
			int minY = (desc->anchorMin >> 2) & 0x3;
			thisBottom = parentBottom + parentHeightHalf * minY + desc->offsetMin.y;

			int maxX = (desc->anchorMax & 0x3);
			thisRight = parentLeft + parentWidthHalf * maxX + desc->offsetMax.x;
			int maxY = (desc->anchorMax >> 2) & 0x3;
			thisTop = parentBottom + parentHeightHalf * maxY + desc->offsetMax.y;
			desc->dirty = 0;
			//desc->p
			for (int j = 0; j < desc->children[0]; j++)
				updateRectTransforms(desc->children[1 + j], thisLeft, thisBottom, thisTop, thisRight);
		}
	}
}