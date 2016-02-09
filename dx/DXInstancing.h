#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "DXShaderStructs.h"
class DXInstancing{
private:
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;

	ID3D11InputLayout* inputLayout;
	

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11Buffer* instanceBuffer;
	ID3D11Buffer* vertexBuffer;
	int instanceCount;
	
	InstanceStruct* instances;

	void initShadersAndInputLayout(void);
	
public:
	DXInstancing(ID3D11Device *_dev, ID3D11DeviceContext *_devcon);
	void init(void);
	void updateInstanceBuffer(void);
	
	void render(void);
	void dispose(void);
	void initUIQuadBuffer(void);
	void initQuadBuffer(void);
	void setQuadVertexBuffer(void);
	void drawQuad(void);
};