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
#include "../misc/CharHelper.h"
#include "../misc/Macros.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

using namespace OriGraphics;
class DirectX11 : public Renderer{
private:
	IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
	ID3D11Device* dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView* backbuffer;

	ID3D11Texture2D* depthStencilTex;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	// instancing
	DXInstancing* instancedDraw;
	DXInstancedMesh* instancedDrawMesh;
	ID3D11Buffer* viewProjMatrixCB;

	struct ViewProjection{

		D3DXMATRIX view;
		D3DXMATRIX projection;

	};

	void initDepthStencil(void);
	void prepareCamera(void);
	void assembleDrawables(void);
public:
	DirectX11(void);
	void init(HWND hWnd, int _width, int _height);
	void dispose(void);

	
	void initInstancing(void);
	void disposeInstancing(void);
	void render(void);
	
};