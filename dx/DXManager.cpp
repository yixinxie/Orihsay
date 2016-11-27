#include "DXManager.h"

#include "../gameplay/Transform.h"
DXManager::DXManager(void){
	//ZeroMemory((void*)this, sizeof(DXManager));
	swapchain = nullptr;
	dev = nullptr;
	devcon = nullptr;
	backbuffer = nullptr;
	
	depthStencilTex = nullptr;
	depthStencilState = nullptr;
	depthStencilView = nullptr;

	instancedQuads = nullptr;
	instancedMesh = nullptr;
	shadowMap = nullptr;
	deferredShading = nullptr;
	viewProjMatrixCB = nullptr;
	
	objectIndexIncrementer = 0; // this should not be here probably...
	spriteIndexIncrementer = 0;
	lightIndexIncrementer = 0;

	
}
void DXManager::dispose()
{
	// close and release all existing COM objects
	// render technique related



	SAFE_DISPOSE(shadowMap);
	SAFE_DISPOSE(instancedSprites);
	SAFE_DISPOSE(instancedMesh);
	SAFE_DISPOSE(instancedQuads);
	SAFE_DISPOSE(deferredShading);

	// depth stencil related
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(depthStencilTex);

	SAFE_RELEASE(swapchain);
	SAFE_RELEASE(backbuffer);
	SAFE_RELEASE(dev);
	SAFE_RELEASE(devcon);
}
void DXManager::init(HWND hWnd, int _width, int _height)
{
	HRESULT hr;
	windowWidth = _width;
	windowHeight = _height;
	printf("init window %d x %d\n", _width, _height);
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.BufferCount = 1;                                    // one back buffer
	scd.OutputWindow = hWnd;                                // the window to be used
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
	printf("create device hr: %d\n", hr);
	// get the address of the back buffer
	ID3D11Texture2D *backBufferTex;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	if (FAILED(hr)){
		
		TRACE("GetBuffer failed");
		
	}
	
	hr = dev->CreateRenderTargetView(backBufferTex, NULL, &backbuffer);
	if (FAILED(hr)){
		TRACE("CreateRenderTargetView failed");
	}
	D3D11_TEXTURE2D_DESC backBufferDesc;
	backBufferTex->GetDesc(&backBufferDesc);
	width = backBufferDesc.Width;
	height = backBufferDesc.Height;
	backBufferTex->Release();
	printf("back buffer size: %d, %d\n", width, height);
	initDepthStencil();

	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	// Set the viewport -----------------camera dependent
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	devcon->RSSetViewports(1, &viewport);
	shadowMap = new DXShadowMap(dev, devcon);
	shadowMap->init(width, height);

	deferredShading = new DXDeferred(dev, devcon);
	deferredShading->init(width, height);

	instancedQuads = new DXInstancing(dev, devcon);
	instancedQuads->initQuadBuffer();

	instancedSprites = new DXInstancedSprite(dev, devcon);
	instancedSprites->init();


}
void DXManager::initDepthStencil(){
	HRESULT hr;
	D3D11_TEXTURE2D_DESC descDepth;
	
	descDepth.Width = width; // questionable! should probably switch to using the values in the buffer desc.
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	
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
	//descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
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
	depthStencilTex->Release();
	
	int sdf = 0;


}

void DXManager::renderWithoutShadowMap(){
	HRESULT hr;
	prepareCamera();
	assembleDrawables();
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//devcon->VSSetConstantBuffers(0, 1, &viewProjMatrixCB);
	instancedMesh->render(&viewProjMatrixCB);
	hr = swapchain->Present(0, 0);
	if (FAILED(hr)){
		TRACE("present failed!");
	}
}
void DXManager::renderWithShadowMap(){
	HRESULT hr;
	assembleDrawables();
	// gather light sources
	// for each light source, render the scene
	if (lightSources.size() > 0){
		// create or update the view projection matrix buffer for the light source.
		
		// render with the depth-only shader.
		LightSourceDesc* lightSourceDesc = lightSources[0];

		shadowMap->setRenderTarget_DepthBuffer();
		shadowMap->updateLightViewCB(lightSourceDesc->position, lightSourceDesc->rotation, 60.0f, (float)width / height, 0.3f, 1000.0f);
		devcon->ClearRenderTargetView(shadowMap->rgbRTV, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		devcon->ClearDepthStencilView(shadowMap->depthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		instancedMesh->renderDepthOnly(&(shadowMap->lightSourceViewProjMatrixCB), shadowMap->vertexShader, shadowMap->pixelShader);
		
		restoreRenderTarget();
		prepareCamera();
		devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		
		instancedMesh->renderWithShadowMap(shadowMap->shadowMapVertexShader, shadowMap->shadowMapPixelShader, shadowMap->rgbSRV, shadowMap->samplerState, &viewProjMatrixCB, &(shadowMap->lightSourceViewProjMatrixCB), &(shadowMap->lightSourcePositionCB));
	}
	else{
		prepareCamera();
		devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		instancedMesh->render(&viewProjMatrixCB);
	}
	hr = swapchain->Present(0, 0);
	if (FAILED(hr)){
		TRACE("present failed!");
	}
}
// render with deferred-shading
void DXManager::platformRender(){
	HRESULT hr;
	assembleDrawables();
	// gather light sources
	// for each light source, render the scene
	
	if (lightSources.size() > 0){
		// create or update the view projection matrix buffer for the light source.

		// render with the depth-only shader.
		LightSourceDesc* lightSourceDesc = lightSources[0];

		shadowMap->setRenderTarget_DepthBuffer();
		shadowMap->updateLightViewCB(lightSourceDesc->position, lightSourceDesc->rotation, 60.0f, (float)width / height, 0.3f, 1000.0f);
		
		instancedMesh->renderDepthOnly(&(shadowMap->lightSourceViewProjMatrixCB), shadowMap->vertexShader, shadowMap->pixelShader);
		////////
		prepareCamera();		
		deferredShading->setRenderTargets_GBuffers();//OMSetRenderTargets
		deferredShading->setShaders();
		instancedMesh->setInputLayout();
		deferredShading->setShaderConstants(&viewProjMatrixCB);
		instancedMesh->setVertexAndIndexBuffers();
		instancedMesh->draw();
		////////
		restoreRenderTarget();
		deferredShading->setInputLayout_Shading();
		instancedQuads->setQuadVertexBuffer();
		deferredShading->setShaders_Shading();
		deferredShading->setShaderConstants_Shading(nullptr, shadowMap->samplerState);
		instancedQuads->drawQuad();
		deferredShading->postDraw();


		//instancedSprites->draw();
	}
	else{
		prepareCamera();
		devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
		devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		instancedMesh->render(&viewProjMatrixCB);
	}
	// 2d rendering
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//prepareCamera();
	
	instancedSprites->render(&viewProjMatrixCB, &textures[0].textureSRV);
	hr = swapchain->Present(0, 0);
	if (FAILED(hr)){
		TRACE("present failed!");
	}
}
void DXManager::restoreRenderTarget(){
	devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//devcon->RSSetViewports(1, &viewport);
	
}
void DXManager::prepareCamera(){
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

void DXManager::assembleDrawables(){
	if (instancedMesh == nullptr){
		instancedMesh = new DXInstancedMesh(dev, devcon);
		instancedMesh->init();
	}
	instancedMesh->updateInstanceBuffer(instancedObjects);

	if (instancedSprites == nullptr){
		instancedSprites = new DXInstancedSprite(dev, devcon);
		instancedSprites->init();
	}
	instancedSprites->updateInstanceBuffer(spriteObjects);

}
int DXManager::createTexture(unsigned int width, unsigned int height, const unsigned char* initialData){

	DXTextureResource tex;


	D3D11_TEXTURE2D_DESC textureDesc;
	
	HRESULT hr;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	ZeroMemory(&textureDesc, sizeof(shaderResourceViewDesc));

	// Setup the texture description.
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// 
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = initialData;
	initData.SysMemPitch = width * 4;
	initData.SysMemSlicePitch = 0;

	// Create the texture.
	hr = dev->CreateTexture2D(&textureDesc, &initData, &(tex.texture));
	if (FAILED(hr)){
		return false;
	}
	
	
	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = dev->CreateShaderResourceView(tex.texture, &shaderResourceViewDesc, &tex.textureSRV);
	if (FAILED(hr)){
		return false;
	}
	int ret = textures.size();
	textures.push_back(tex);

	return ret;
}