#pragma once
#include <d3dx11.h>
#include "../math/EasyMath.h"
// shader constant buffer structs
struct ViewProjection{

	D3DXMATRIX view;
	D3DXMATRIX projection;
};

// vertex buffer structs
struct InstanceStruct{
	D3DXMATRIX worldMatrix;
};
//
struct PerVertexData{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
	D3DXVECTOR3 normal;
};
struct SpriteVertexData{
	Vector3 position;
	Vector2 uv;
};

struct PerVertexData_Quad{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
};