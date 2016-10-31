#pragma once
struct Vector2{
	float x, y;
	Vector2(const float _x, const float _y) :x(_x), y(_y) {
	}
	Vector2() :x(0), y(0){}

	Vector2 operator*(const float val)
	{
		Vector2 vec;
		vec.x = this->x * val;
		vec.y = this->y * val;
		return vec;
	}
	Vector2 operator+=(const Vector2& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	Vector2 operator-=(const Vector2& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

};
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
	Vector4(const float _x, const float _y, const float _z, const float _w) :x(_x), y(_y), z(_z), w(_w) {
	}
	Vector4(const Vector3 vec) :x(vec.x), y(vec.y), z(vec.z), w(1) {
	}
};
struct IntVector2{
	int x, y;
	IntVector2(const int _x, const int _y) :x(_x), y(_y){
	}
	IntVector2(const long _x, const long _y) :x(_x), y(_y){
	}
	IntVector2() :x(0), y(0){}
};
