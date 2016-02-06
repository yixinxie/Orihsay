#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <unordered_map>
#include "../math/EasyMath.h"
#include "../graphics/Renderer.h"
using namespace OriGraphics;
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
	int instanceMaxSize;
	// per instance data
	struct InstanceStruct{
		D3DXMATRIX worldMatrix;
	};
	InstanceStruct* instances;

	struct PerVertexData{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
	struct PerVertexDataDepthOnly{
		D3DXVECTOR3 position;
	};
	
	void initShadersAndInputLayout(void);
	void initCubeBuffer(void);
	void initInstanceBuffer(void);
public:
	DXInstancedMesh(ID3D11Device *_dev, ID3D11DeviceContext *_devcon);
	void init(void);
	void updateInstanceBuffer(const std::unordered_map<int, ObjectTransformDesc*>& instancedObjects);
	void render(ID3D11Buffer** viewProjCB);
	void renderDepthOnly(ID3D11Buffer** viewProjCB, ID3D11VertexShader* depthVertexShader, ID3D11PixelShader* depthPixelShader);
	void renderWithShadowMap(ID3D11VertexShader* shadowVertexShader, ID3D11PixelShader* shadowPixelShader, ID3D11ShaderResourceView* shadowMapSRV, ID3D11SamplerState* samplerState, ID3D11Buffer** viewProjCB, ID3D11Buffer** lightViewProjCB, ID3D11Buffer** lightPositionCB);
	void dispose(void);
};