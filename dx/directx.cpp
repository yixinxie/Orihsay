#include "directx.h"
DirectX11::DirectX11(void){
}
void DirectX11::init(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);


	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target--------set once per camera
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer -----------camera dependent
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// Set the viewport -----------------camera dependent
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
}
void DirectX11::close()
{
	// close and release all existing COM objects
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}



void DirectX11::initInstancing(){
	instancedDraw = new DXInstancing(dev, devcon);
	instancedDraw->init();
}
void DirectX11::disposeInstancing(){
	SAFE_DISPOSE(instancedDraw);
}
void DirectX11::renderWithInstancing(){
	HRESULT hr;
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	instancedDraw->render();
	hr = swapchain->Present(1, 0);
	if (FAILED(hr)){
		TRACE("present failed!");
	}
}