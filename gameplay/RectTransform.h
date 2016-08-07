#pragma once

class GameObject;
#include "../math/EasyMath.h"
#include "glm/gtc/quaternion.hpp"
#include "ITransform.h"
class RectTransform : public ITransform{
private:
	
	GameObject* gameObject;
public:
	Vector2 position; // anchored position
	Vector2 widthHeight; // sizeDelta
	Vector2 anchorMin, anchorMax;
	Vector2 pivot; // pivot uv.
	float rotation; // z rotation around the pivot.
	Vector2 scale;
	
	RectTransform(void);
	void setGameObject(GameObject* _go);

};
