#include "directx.h"
DirectX11::DirectX11(void){
	viewProjMatrixCB = nullptr;
}
void DirectX11::init(HWND hWnd, int _width, int _height)
{
	width = _width;
	height = _height;
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
	viewport.Width = (int)width;
	viewport.Height = (int)height;

	devcon->RSSetViewports(1, &viewport);
}
void DirectX11::dispose()
{
	// close and release all existing COM objects
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}



void DirectX11::initInstancing(){
	/*instancedDraw = new DXInstancing(dev, devcon);
	instancedDraw->init();*/

	instancedDrawMesh = new DXInstancedMesh(dev, devcon);
	instancedDrawMesh->init();
}
void DirectX11::disposeInstancing(){
	//SAFE_DISPOSE(instancedDraw);
	SAFE_DISPOSE(instancedDrawMesh);
}
void DirectX11::render(){
	HRESULT hr;
	prepareCamera();
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	//devcon->VSSetConstantBuffers(0, 1, &viewProjMatrixCB);
	instancedDrawMesh->render(&viewProjMatrixCB);
	hr = swapchain->Present(1, 0);
	if (FAILED(hr)){
		TRACE("present failed!");
	}
}
void DirectX11::prepareCamera(){
	if (cameras.size() == 0){
		TRACE("No camera found!");
		return;
	}
	CameraParameters* camParams = cameras.at(0);
	

	D3DXVECTOR3 camPos(camParams->position.x, camParams->position.y, camParams->position.z);
	D3DXVECTOR3 lookAt(0, 0, 0); // undefined!
	D3DXVECTOR3 up(0, 1, 0); // should be infered from the orientation of the camera.

	ViewProjection wvp;
	D3DXMatrixLookAtLH(&(wvp.view), &camPos, &lookAt, &up);
	D3DXMatrixPerspectiveFovLH(&(wvp.projection), D3DXToRadian(camParams->fieldOfView), (float)width / height, camParams->nearPlane, camParams->farPlane);
	D3DXMatrixTranspose(&(wvp.view), &(wvp.view));
	D3DXMatrixTranspose(&(wvp.projection), &(wvp.projection));

	if (viewProjMatrixCB != nullptr){

		D3D11_BUFFER_DESC cbd = { sizeof(wvp), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0 };
		D3D11_SUBRESOURCE_DATA cbdInitData = { &wvp, 0, 0 };

		HRESULT hr = dev->CreateBuffer(&cbd, &cbdInitData, &viewProjMatrixCB);
		if (FAILED(hr)){
			TRACE("camera constant buffer create failed!");
		}
		
	}
	
}