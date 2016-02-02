#pragma once

struct Vector3{
	float x, y, z;
	Vector3(const float _x, const float _y, const float _z) :x(_x), y(_y), z(_z) {
	}
	Vector3() :x(0), y(0), z(0){}

	Vector3 operator*(const float val)
	{
		Vector3 vec;
		vec.x = this->x * val;
		vec.y = this->y * val;
		vec.z = this->z * val;
		return vec;
	}
	Vector3 operator+=(const Vector3& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
	Vector3 operator-=(const Vector3& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

};
struct Vector4{
	float x, y, z, w;
};
struct IntVector2{
	int x, y;
	IntVector2(const int _x, const int _y) :x(_x), y(_y){
	}
	IntVector2(const long _x, const long _y) :x(_x), y(_y){
	}
	IntVector2() :x(0), y(0){}
};