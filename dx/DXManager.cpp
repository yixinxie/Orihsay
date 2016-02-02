#include "DXManager.h"
#include "../gameplay/Transform.h"
DirectX11::DirectX11(void){
	
	swapchain = nullptr;
	dev = nullptr;
	devcon = nullptr;

	backbuffer = nullptr;
	
	depthStencilTex = nullptr;
	depthStencilState = nullptr;
	depthStencilView = nullptr;

	instancedDraw = nullptr;
	instancedDrawMesh = nullptr;
	viewProjMatrixCB = nullptr;
	objectIndexIncrementer = 0; // this should not be here probably...
	
}
void DirectX11::init(HWND hWnd, int _width, int _height)
{
	HRESULT hr;
	windowWidth = _width;
	windowHeight = _height;
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);
	if (FAILED(hr)){
		TRACE("create device failed");
	}

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)){
		TRACE("GetBuffer failed");
	}
	
	hr = dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	if (FAILED(hr)){
		TRACE("CreateRenderTargetView failed");
	}
	D3D11_TEXTURE2D_DESC backBufferDesc;
	pBackBuffer->GetDesc(&backBufferDesc);
	width = backBufferDesc.Width;
	height = backBufferDesc.Height;
	pBackBuffer->Release();
	initDepthStencil();
	
	
	//devcon->OMSetRenderTargets(1, &backbuffer, NULL);
	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	// Set the viewport -----------------camera dependent
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	devcon->RSSetViewports(1, &viewport);

	
}
void DirectX11::initDepthStencil(){
	HRESULT hr;
	D3D11_TEXTURE2D_DESC descDepth;
	
	descDepth.Width = width; // questionable! should probably switch to using the values in the buffer desc.
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = dev->CreateTexture2D(&descDepth, NULL, &depthStencilTex);
	if (FAILED(hr)){
		TRACE("create depth stencil texture failed!");
	}


	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state

	hr = dev->CreateDepthStencilState(&dsDesc, &depthStencilState);
	if (FAILED(hr)){
		TRACE("create depth stencil state failed!");
	}

	devcon->OMSetDepthStencilState(depthStencilState, 1);


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	
	//descDSV.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	
	hr = dev->CreateDepthStencilView(depthStencilTex, // Depth stencil texture
		&descDSV, // Depth stencil desc
		&depthStencilView);  // [out] Depth stencil view
	if (FAILED(hr)){
		TRACE("CreateDepthStencilView failed!");
	}
	//devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);
}
void DirectX11::dispose()
{
	// close and release all existing COM objects
	SAFE_DISPOSE(instancedDrawMesh);

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
	assembleDrawables();
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
	CameraDesc* camParams = cameras.at(0);

	D3DXVECTOR3 camPos(camParams->position.x, camParams->position.y, camParams->position.z);

	Vector3 _lookat, _up;

	//Transform::getLookatAndUp(Vector3(glm::radians(0.0f), glm::radians(0.0), glm::radians(30.0f)), &_lookat, &_up);

	Transform::getLookatAndUp(camParams->rotation, &_lookat, &_up);

	D3DXVECTOR3 lookAt(camParams->position.x + _lookat.x, camParams->position.y + _lookat.y, camParams->position.z + _lookat.z); // undefined!
	D3DXVECTOR3 up(_up.x, _up.y, _up.z); // should be infered from the orientation of the camera.

	ViewProjection wvp;
	D3DXMatrixLookAtLH(&(wvp.view), &camPos, &lookAt, &up);
	D3DXMatrixPerspectiveFovLH(&(wvp.projection), D3DXToRadian(camParams->fieldOfView), (float)width / height, camParams->nearPlane, camParams->farPlane);
	
	HRESULT hr;
	if (viewProjMatrixCB == nullptr){
		D3DXMatrixTranspose(&(wvp.view), &(wvp.view));
		D3DXMatrixTranspose(&(wvp.projection), &(wvp.projection));
		D3D11_BUFFER_DESC cbd = { sizeof(wvp), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0 };
		D3D11_SUBRESOURCE_DATA cbdInitData = { &wvp, 0, 0 };

		hr = dev->CreateBuffer(&cbd, &cbdInitData, &viewProjMatrixCB);
		if (FAILED(hr)){
			TRACE("camera constant buffer create failed!");
		}
	}
	else {
		// if the view matrix buffer is already created, we just need to update it.
		D3D11_MAPPED_SUBRESOURCE resource;
		hr = devcon->Map(viewProjMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		ViewProjection* viewProjectionMatrices = (ViewProjection*)resource.pData;
		D3DXMatrixTranspose(&(viewProjectionMatrices->view), &(wvp.view));
		D3DXMatrixTranspose(&(viewProjectionMatrices->projection), &(wvp.projection));

		devcon->Unmap(viewProjMatrixCB, 0);

	}

	
}
void DirectX11::assembleDrawables(){
	if (instancedDrawMesh == nullptr){
		instancedDrawMesh = new DXInstancedMesh(dev, devcon);
		instancedDrawMesh->init();
	}
	instancedDrawMesh->updateInstanceBuffer(instancedObjects);

}