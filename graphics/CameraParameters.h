#pragma once
#include "../math/EasyMath.h"
struct CameraParameters{
	// view matrix related:
	Vector3 position, rotation;
	// projection matrix related:
	int projectionMode; // perspective, orthogonal
	float fieldOfView;
	float nearPlane, farPlane;
};