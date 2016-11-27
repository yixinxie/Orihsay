#pragma once
#include "../math/EasyMath.h"
struct ObjectTransformDesc{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};
struct ObjectRectTransformDesc{
	unsigned char dirty;
	Vector2 position; // pivot position
	Vector2 rectMin;
	Vector2 rectMax;
	Vector2 rotation;
	Vector2 scale;
	int textureHandle;
	int* children; // the first element indicates the size.
	int parent;
	
	unsigned char anchorMin; // low 4 bit for x,
	unsigned char anchorMax;
	IntVector2 offsetMin;
	IntVector2 offsetMax;
	IntVector2 pivot; // range 0..1
};