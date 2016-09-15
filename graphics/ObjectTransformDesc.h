#pragma once
#include "../math/EasyMath.h"
struct ObjectTransformDesc{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};
struct ObjectRectTransformDesc{
	
	Vector2 position; // pivot position
	Vector2 widthHeight;
	int textureHandle;
	Vector2 pivot; // range 0..1
	Vector2 anchor;
	Vector2 rotation;
	Vector2 scale;
};