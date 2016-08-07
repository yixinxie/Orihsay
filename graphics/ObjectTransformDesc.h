#pragma once
#include "../math/EasyMath.h"
struct ObjectTransformDesc{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};
struct ObjectRectTransformDesc{
	Vector2 position; // pivot position
	Vector2 pivot;
	Vector2 widthHeight;
	Vector2 anchor;
	Vector2 rotation;
	Vector2 scale;
};