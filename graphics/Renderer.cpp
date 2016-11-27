#include "Renderer.h"
#include <string>
#include <unordered_map>
#include "../misc/CharHelper.h"
#include "../misc/BasicMem.h"
using namespace OriGraphics;
void Renderer::setMainCamera(const Vector3& pos, const Vector3& rot, const float fov, const float _nearPlane, const float _farPlane){
	CameraDesc* params = ori_alloc(CameraDesc);
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
	ObjectTransformDesc* oit = ori_alloc(ObjectTransformDesc);
	oit->cns();
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
	LightSourceDesc* lightSourceDesc = ori_alloc(LightSourceDesc);

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
void Renderer::updateRectTransforms(int idx, int parentLeft, int parentBottom, int parentRight, int parentTop){
	const int multipliers[] = {
		0, 0, 1, 0, 2, 0,
		0, 1, 1, 1, 2, 1,
		0, 2, 1, 2, 2, 2,
	};
	ObjectRectTransformDesc* desc = spriteObjects.at(idx);
	if (desc->dirty != 0){
		// this is to help calculate the min/max anchor positions.
			
		int parentWidthHalf = (parentRight - parentLeft) / 2;
		int parentHeightHalf = (parentTop - parentBottom) / 2;

		int rectLeft, rectBottom, rectRight, rectTop;
		int minX = multipliers[desc->anchorMin * 2];
		// parentLeft + parentWidthHalf is the anchor left
		rectLeft = parentLeft + parentWidthHalf * minX + desc->offsetMin.x;
		int minY = multipliers[desc->anchorMin * 2 + 1];
		rectBottom = parentBottom + parentHeightHalf * minY + desc->offsetMin.y;

		int maxX = multipliers[desc->anchorMax * 2];
		rectRight = parentLeft + parentWidthHalf * maxX + desc->offsetMax.x;
		int maxY = multipliers[desc->anchorMax * 2 + 1];
		rectTop = parentBottom + parentHeightHalf * maxY + desc->offsetMax.y;
		// now (rectRight - rectLeft, rectTop - rectBottom) is the sizeDelta of the rect transform.

		desc->rectMin = Vector2(rectLeft, rectBottom);
		desc->rectMax = Vector2(rectRight, rectTop);

		desc->dirty = 0;
		//desc->p
		for (int j = 0; j < desc->children.length; ++j)
			if (desc->children[j] >= 0)
				updateRectTransforms(desc->children[j], rectLeft, rectBottom, rectRight, rectTop);
	}
}
void Renderer::preRender(){
	

}
int Renderer::registerSpriteObject(){
	int res;
	ObjectRectTransformDesc* desc = ori_alloc(ObjectRectTransformDesc);
	desc->cns();
	spriteObjects.insert({ spriteIndexIncrementer, desc });
	res = spriteIndexIncrementer;
	spriteIndexIncrementer++;
	return res;
}
void Renderer::updateSpriteObjectParent(const int id, const int parentId){
	

	// here we first try to determine if the current node is already in newParent's children list.
	ObjectRectTransformDesc* parentDesc = spriteObjects[parentId];
	bool found = false;
	for (int i = 0; i < parentDesc->children.length; ++i){
		if (parentDesc->children[i] == id){
			found = true;
			break;
		}
	}
	if (found == false){
		parentDesc->dirty = 1;
		parentDesc->children.push(id);
		// detach from the old parrent.
		ObjectRectTransformDesc* current = spriteObjects[id];
		//current->parent = parentId;
		if (current->parent >= 0){
			ObjectRectTransformDesc* oldParentDesc = spriteObjects[parentId];

			oldParentDesc->children.remove(id);
		}
		current->parent = parentId;
	}
}
void Renderer::updateSpriteObject(const int id, RectTransform* rect){

	ObjectRectTransformDesc* desc = spriteObjects[id];
	if (desc == nullptr){
		TRACE("cannot find a sprite object with id" << id);
		return;
	}
	desc->dirty = 1;
	desc->anchorMin = rect->anchorMin;
	desc->anchorMax = rect->anchorMax;
	desc->offsetMin = rect->offsetMin;
	desc->offsetMax = rect->offsetMax;
	desc->pivot = rect->pivot;

	
}
void Renderer::render(){
	//pre render
	updateRectTransforms(0, 0, 0, width, height);
	platformRender();
	postRender();
}
void Renderer::postRender(){

}