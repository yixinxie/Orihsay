#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <unordered_map>
#include "../math/EasyMath.h"
#include "../graphics/Renderer.h"
#include "DXShaderStructs.h"
using namespace OriGraphics;
class DXInstancedSprite{
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
	int instanceMaxSize;
	// per instance data
	
	InstanceStruct* instances;
	
	void initShadersAndInputLayout(void);
	void initInstanceBuffer(void);
public:
	DXInstancedSprite(ID3D11Device *_dev, ID3D11DeviceContext *_devcon);
	void init(void);
	void updateInstanceBuffer(const std::unordered_map<int, ObjectTransformDesc*>& instancedObjects);
	void render(ID3D11Buffer** viewProjCB);
	void dispose(void);

	void setVertexAndIndexBuffers(void);
	void draw(void);
	void setInputLayout(void);
};