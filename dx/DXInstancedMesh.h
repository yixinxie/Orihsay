#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
class DXInstancedMesh{
private:
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;

	ID3D11InputLayout* inputLayout;
	

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11Buffer* instanceBuffer;
	
	int instanceCount;
	// per instance data
	struct InstanceStruct{
		D3DXVECTOR3 position;
	};
	InstanceStruct* instances;

	struct PerVertexData{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	//struct MatrixBufferStruct
	//{
	//	D3DXMATRIX world;
	//	D3DXMATRIX view;
	//	D3DXMATRIX projection;
	//};
	
	//MatrixBufferStruct matrixBuffer;
	void initShadersAndInputLayout(void);
	void initCubeBuffer(void);
public:
	DXInstancedMesh(ID3D11Device *_dev, ID3D11DeviceContext *_devcon);
	void init(void);
	void updateInstanceBuffer(void);
	
	void render(void);
	void dispose(void);
};