#include "DXShadowMap.h"
#include "../misc/Macros.h"
#include "../misc/CharHelper.h"
#include "../math/EasyMath.h"

DXShadowMap::DXShadowMap(ID3D11Device* _dev, ID3D11DeviceContext* _devcon) :
rgbTex(nullptr), rgbRTV(nullptr), rgbSRV(nullptr), depthTex(nullptr), depthDSV(nullptr) {
	dev = _dev;
	devcon = _devcon;
	shadowMapVertexShader = nullptr;
	shadowMapPixelShader = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
}
void DXShadowMap::initShaders(){
	HRESULT hr;
	// shader

	CharBuffer* vsBuffer = CharHelper::loadFile("instanced_shadowmap_depth_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);
	if (FAILED(hr)){
		TRACE("shadowmap depth vertex shader create failed");
	}
	SAFE_DISPOSE(vsBuffer);

	CharBuffer* psBuffer = CharHelper::loadFile("instanced_shadowmap_depth_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);
	if (FAILED(hr)){
		TRACE("shadowmap depth pixel shader create failed");
	}
	SAFE_DISPOSE(psBuffer);


	vsBuffer = CharHelper::loadFile("instanced_shadowmap_mesh_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &shadowMapVertexShader);
	if (FAILED(hr)){
		TRACE("shadowmap color vertex shader create failed");
	}
	SAFE_DISPOSE(vsBuffer);

	psBuffer = CharHelper::loadFile("instanced_shadowmap_mesh_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &shadowMapPixelShader);
	if (FAILED(hr)){
		TRACE("shadowmap color pixel shader create failed");
	}
	SAFE_DISPOSE(psBuffer);
}
void DXShadowMap::init(int width, int height){
	HRESULT hr;
	D3D11_TEXTURE2D_DESC rgbTexDesc;

	rgbTexDesc.Width = width;
	rgbTexDesc.Height = height;
	rgbTexDesc.MipLevels = 1;
	rgbTexDesc.ArraySize = 1;
	rgbTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	rgbTexDesc.SampleDesc.Count = 1;
	rgbTexDesc.SampleDesc.Quality = 0;
	rgbTexDesc.Usage = D3D11_USAGE_DEFAULT;
	rgbTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	rgbTexDesc.CPUAccessFlags = 0;
	rgbTexDesc.MiscFlags = 0;

	hr = dev->CreateTexture2D(&rgbTexDesc, NULL, &rgbTex);
	if (FAILED(hr)){
		TRACE("create shadow rgb texture failed!");
	}


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = rgbTexDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = dev->CreateRenderTargetView(rgbTex, &renderTargetViewDesc, &rgbRTV);
	if (FAILED(hr)){
		TRACE("create shadow rgb RTV failed!");
	}

	// shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = rgbTexDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = dev->CreateShaderResourceView(rgbTex, &shaderResourceViewDesc, &rgbSRV);
	if (FAILED(hr)){
		TRACE("create shadow rgb SRV failed!");
	}

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
		TRACE("create shadow depth tex failed!");
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
	// init the viewport
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	initShaders();

}
void DXShadowMap::prepareLightView(){
	devcon->OMSetRenderTargets(1, &rgbRTV, depthDSV);

	devcon->RSSetViewports(1, &viewport);
	float clearColor[4] = { 0, 0, 0, 1 };

	devcon->ClearRenderTargetView(rgbRTV, clearColor);

	// Clear the depth buffer.
	devcon->ClearDepthStencilView(depthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void DXShadowMap::dispose(){
	SAFE_RELEASE(rgbTex);
	SAFE_RELEASE(rgbRTV);
	SAFE_RELEASE(rgbSRV);
	SAFE_RELEASE(depthTex);
	SAFE_RELEASE(depthDSV);
}