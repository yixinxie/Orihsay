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

	// core values
	unsigned char anchorMin, anchorMax;
	IntVector2 offsetMin, offsetMax;
	Vector2 pivot; // pivot uv.
	// end of core values.

	float rotation; // z rotation around the pivot.
	Vector2 scale;

	
	RectTransform(void);
	void setGameObject(GameObject* _go);
	GameObject* getGameObject(void);
	void setParent(int val);
	void addChild(int val);

};
