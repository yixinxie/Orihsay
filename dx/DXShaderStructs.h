#pragma once
#include <d3dx11.h>
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

struct PerVertexData_Quad{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
};