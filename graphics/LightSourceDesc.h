#pragma once
#include "../math/EasyMath.h"
struct LightSourceDesc{
	Vector3 position, rotation;

	int type; // point light, directional light, ambient light etc
	int shadowMode; // 0 - none, 1 - shadow map, 2 - cascaded shadow map?
	Vector3 color;
	float intensity;
};