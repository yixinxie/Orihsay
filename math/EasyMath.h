#include "D3DX10math.h"
struct Vector3{
	float x, y, z;
	Vector3(D3DXVECTOR3 vec){
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	Vector3() :x(0), y(0), z(0){}

};
struct Vector4{
	float x, y, z, w;
};