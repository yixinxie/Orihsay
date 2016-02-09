// include the basic windows header files and the Direct3D header files
#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "../graphics/Renderer.h"
#include "DXInstancing.h"
#include "DXInstancedMesh.h"
#include "DXShadowMap.h"
#include "DXDeferred.h"
#include "../misc/CharHelper.h"
#include "../misc/Macros.h"
#include "DXShaderStructs.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

using namespace OriGraphics;
class DXManager : public Renderer{
private:
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView* backbuffer;

	D3D11_VIEWPORT viewport;

	// depth and stencil
	ID3D11Texture2D* depthStencilTex;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	// instancing
	ID3D11Buffer* viewProjMatrixCB;

	// techniques:
	DXInstancing* instancedQuads;
	DXInstancedMesh* instancedMesh;
	DXShadowMap* shadowMap;
	DXDeferred* deferredShading;

	void initDepthStencil(void);
	void prepareCamera(void);
	void prepareViewProjectionCB(ID3D11Buffer** constantBuffer, const Vector3 position, const Vector3 euler, float fieldOfView, float aspectRatio, float nearPlane, float farPlane, ID3D11Buffer** constantBuffer2);
	void assembleDrawables(void);
	void restoreRenderTarget(void);
public:
	DXManager(void);
	void init(HWND hWnd, int _width, int _height);
	void dispose(void);

	
	void initInstancing(void);
	void disposeInstancing(void);
	void render(void);

	// experimental
	void renderWithShadowMap(void);
	void renderWithoutShadowMap(void);
};