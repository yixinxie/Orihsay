#pragma once

struct Vector3{
	float x, y, z;
	Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {
	}
	Vector3() :x(0), y(0), z(0){}

};
struct Vector4{
	float x, y, z, w;
};