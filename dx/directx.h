// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "../misc/CharHelper.h"

#define SAFE_RELEASE(h) if(h != nullptr){(h)->Release();}
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


	ID3D11InputLayout* inputlayout;
	ID3D11Buffer* vertexbuffer;

	ID3D11VertexShader* vertexshader;
	ID3D11PixelShader* pixelshader;
public:
	DirectX11(void);
	void init(HWND hwnd);
	void close(void);
	void renderFrame(void);

	void initQuadBuffer(void);
	void disposeQuadBuffer(void);
	CharBuffer* LoadShaderFile(std::string File);
};