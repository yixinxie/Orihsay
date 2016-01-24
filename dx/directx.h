// include the basic windows header files and the Direct3D header files
#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "DXInstancing.h"
#include "../misc/CharHelper.h"

#define SAFE_RELEASE(h) if(h != nullptr){(h)->Release(); (h) = nullptr;}

#define SAFE_DISPOSE(h) if((h) != nullptr){(h)->dispose(); delete (h); (h) = nullptr;}
// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class DirectX11{
private:
	IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context

	ID3D11RenderTargetView *backbuffer;

	// instancing
	DXInstancing* instancedDraw;
public:
	DirectX11(void);
	void init(HWND hwnd);
	void close(void);
	void renderFrame(void);

	void initInstancing(void);
	void disposeInstancing(void);
	void renderWithInstancing(void);

};