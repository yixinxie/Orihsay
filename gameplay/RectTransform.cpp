#include "RectTransform.h"
#include "glm/gtx/rotate_vector.hpp"
RectTransform::RectTransform(){
	gameObject = nullptr;
	position = Vector2(0, 0);
	widthHeight = Vector2(0, 0);
	anchorMin = Vector2(0, 0);
	anchorMax = Vector2(0, 0);
	pivot = Vector2(0.5f, 0.5f);
	rotation = 0;
	scale = Vector2(1, 1);
}

void RectTransform::setGameObject(GameObject* _go){
	gameObject = _go;
}