#include "DXDeferred.h"
#include "../misc/CharHelper.h"
#include "../misc/Macros.h"
DXDeferred::DXDeferred(ID3D11Device* _dev, ID3D11DeviceContext* _devcon) :dev(_dev), devcon(_devcon){
	for (int i = 0; i < DEFERRED_G_BUFFER_COUNT; i++){
		rgbTex[i] = nullptr;
		rgbSRV[i] = nullptr;
		rgbRTV[i] = nullptr;
	}
	depthTex = nullptr;
	depthDSV = nullptr;

	vertexShader = nullptr;
	pixelShader = nullptr;

	combineVertexShader = nullptr;
	combinePixelShader = nullptr;
}
void DXDeferred::dispose(){

	for (int i = 0; i < DEFERRED_G_BUFFER_COUNT; i++){
		SAFE_RELEASE(rgbSRV[i]);
		SAFE_RELEASE(rgbRTV[i]);
		SAFE_RELEASE(rgbTex[i]);
	}

	SAFE_RELEASE(depthTex);
	SAFE_RELEASE(depthDSV);

	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);

	SAFE_RELEASE(combineVertexShader);
	SAFE_RELEASE(combinePixelShader);
}
void DXDeferred::init(int width, int height){
	initShaders();
	initGBuffers(width, height);
	initDepthStencil(width, height);
}
void DXDeferred::initGBuffers(int width, int height){
	HRESULT hr;
	/*
	in the order of diffuse(RGB), occlusion(A)
	specular color(RGB), roughness(A)
	world space normal(R10 G10 B10)
	emission, lighting, lightmaps, reflection probe buffer
	*/
	DXGI_FORMAT bufferFormats[DEFERRED_G_BUFFER_COUNT] = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM,
	};
	for (int i = 0; i < DEFERRED_G_BUFFER_COUNT; i++){
		D3D11_TEXTURE2D_DESC rgbTexDesc;

		rgbTexDesc.Width = width;
		rgbTexDesc.Height = height;
		rgbTexDesc.MipLevels = 1;
		rgbTexDesc.ArraySize = 1;
		rgbTexDesc.Format = bufferFormats[i];

		rgbTexDesc.SampleDesc.Count = 1;
		rgbTexDesc.SampleDesc.Quality = 0;
		rgbTexDesc.Usage = D3D11_USAGE_DEFAULT;
		rgbTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		rgbTexDesc.CPUAccessFlags = 0;
		rgbTexDesc.MiscFlags = 0;

		hr = dev->CreateTexture2D(&rgbTexDesc, NULL, &rgbTex[i]);
		if (FAILED(hr)){
			TRACE("create deferred rgb texture failed at " << i);
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = rgbTexDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		hr = dev->CreateRenderTargetView(rgbTex[i], &renderTargetViewDesc, &rgbRTV[i]);
		if (FAILED(hr)){
			TRACE("create deferred rgb RTV failed at "<<i);
		}

		// shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = rgbTexDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = dev->CreateShaderResourceView(rgbTex[i], &shaderResourceViewDesc, &rgbSRV[i]);
		if (FAILED(hr)){
			TRACE("create deferred rgb SRV failed at " << i);
		}
	}
}
// there is only one depthstencil buffer needed.
void DXDeferred::initDepthStencil(int width, int height){
	HRESULT hr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = dev->CreateTexture2D(&depthBufferDesc, NULL, &depthTex);
	if (FAILED(hr)){
		TRACE("create deferred depth tex failed!");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = dev->CreateDepthStencilView(depthTex, &depthStencilViewDesc, &depthDSV);
	if (FAILED(hr)){
		TRACE("create shadow depth view failed!");
	}
}
void DXDeferred::initShaders(){
	HRESULT hr;
	// shader

	CharBuffer* vsBuffer = CharHelper::loadFile("instanced_deferred_gbuffer_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);
	if (FAILED(hr)){
		TRACE("shadowmap depth vertex shader create failed");
	}
	
	CharBuffer* psBuffer = CharHelper::loadFile("instanced_deferred_gbuffer_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);
	if (FAILED(hr)){
		TRACE("shadowmap depth pixel shader create failed");
	}
	
	D3D11_INPUT_ELEMENT_DESC instancedDeferredLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INSTANCE_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	// Get a count of the elements in the layout.
	int numElements = sizeof(instancedDeferredLayout) / sizeof(instancedDeferredLayout[0]);

	// Create the vertex input layout.
	hr = dev->CreateInputLayout(instancedDeferredLayout, numElements, vsBuffer->buffer, vsBuffer->length,
		&inputLayout);
	if (FAILED(hr)){
		TRACE("input layout create error!");
	}
	SAFE_DISPOSE(vsBuffer);
	SAFE_DISPOSE(psBuffer);


	vsBuffer = CharHelper::loadFile("deferred_quad_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &combineVertexShader);
	if (FAILED(hr)){
		TRACE("deferred shading quad vertex shader create failed");
	}

	psBuffer = CharHelper::loadFile("deferred_quad_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &combinePixelShader);
	if (FAILED(hr)){
		TRACE("deferred shading quad pixel shader create failed");
	}

	D3D11_INPUT_ELEMENT_DESC quadLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Get a count of the elements in the layout.
	numElements = sizeof(quadLayoutDesc) / sizeof(quadLayoutDesc[0]);

	// Create the vertex input layout.
	hr = dev->CreateInputLayout(quadLayoutDesc, numElements, vsBuffer->buffer, vsBuffer->length,
		&quadInputLayout);
	if (FAILED(hr)){
		TRACE("quad input layout create error!");
	}
	SAFE_DISPOSE(vsBuffer);
	SAFE_DISPOSE(psBuffer);
}
void DXDeferred::setInputLayout_GBuffers(){
	devcon->IASetInputLayout(inputLayout);
}
void DXDeferred::setRenderTargets_GBuffers(){
	float clearColor[4] = {0,0,0,1};

	devcon->OMSetRenderTargets(DEFERRED_G_BUFFER_COUNT, rgbRTV, depthDSV);
	for (int i = 0; i < DEFERRED_G_BUFFER_COUNT; i++){
		devcon->ClearRenderTargetView(rgbRTV[i], clearColor);
	}
	devcon->ClearDepthStencilView(depthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void DXDeferred::setShaders(){
	devcon->VSSetShader(vertexShader, nullptr, 0);
	devcon->PSSetShader(pixelShader, nullptr, 0);
}
void DXDeferred::setShaderConstants(ID3D11Buffer** viewProjCB){
	devcon->VSSetConstantBuffers(0, 1, viewProjCB);
}

void DXDeferred::setInputLayout_Shading(){
	devcon->IASetInputLayout(quadInputLayout);
}
void DXDeferred::setShaders_Shading(){
	devcon->VSSetShader(combineVertexShader, nullptr, 0);
	devcon->PSSetShader(combinePixelShader, nullptr, 0);
}
void DXDeferred::setShaderConstants_Shading(ID3D11Buffer** viewProjCB, ID3D11SamplerState* sampler){
	//devcon->VSSetConstantBuffers(0, 1, viewProjCB);
	devcon->PSSetShaderResources(0, DEFERRED_G_BUFFER_COUNT, rgbSRV);
	ID3D11SamplerState* samplers[DEFERRED_G_BUFFER_COUNT];
	for (int i = 0; i < DEFERRED_G_BUFFER_COUNT; i++){
		samplers[i] = sampler;
	}
	devcon->PSSetSamplers(0, DEFERRED_G_BUFFER_COUNT, samplers);
}
void DXDeferred::postDraw(){
	ID3D11ShaderResourceView* empty[DEFERRED_G_BUFFER_COUNT];
	for (int i = 0; i < DEFERRED_G_BUFFER_COUNT; i++){
		empty[i] = nullptr;
	}
	devcon->PSSetShaderResources(0, DEFERRED_G_BUFFER_COUNT, empty);
}