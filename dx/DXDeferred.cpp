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

	hr = dev->CreateTexture2D(&rgbTexDesc, NULL, &rgbTex[0]);
	if (FAILED(hr)){
		TRACE("create deferred rgb texture failed!");
	}


	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = rgbTexDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = dev->CreateRenderTargetView(rgbTex[0], &renderTargetViewDesc, &rgbRTV[0]);
	if (FAILED(hr)){
		TRACE("create deferred rgb RTV failed!");
	}

	// shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = rgbTexDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = dev->CreateShaderResourceView(rgbTex[0], &shaderResourceViewDesc, &rgbSRV[0]);
	if (FAILED(hr)){
		TRACE("create deferred rgb SRV failed!");
	}
}
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

	CharBuffer* vsBuffer = CharHelper::loadFile("deferred_g_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);
	if (FAILED(hr)){
		TRACE("shadowmap depth vertex shader create failed");
	}
	SAFE_DISPOSE(vsBuffer);

	CharBuffer* psBuffer = CharHelper::loadFile("deferred_g_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);
	if (FAILED(hr)){
		TRACE("shadowmap depth pixel shader create failed");
	}
	SAFE_DISPOSE(psBuffer);

	vsBuffer = CharHelper::loadFile("deferred_combine_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &combineVertexShader);
	if (FAILED(hr)){
		TRACE("shadowmap color vertex shader create failed");
	}
	SAFE_DISPOSE(vsBuffer);

	psBuffer = CharHelper::loadFile("deferred_combine_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &combinePixelShader);
	if (FAILED(hr)){
		TRACE("shadowmap color pixel shader create failed");
	}
	SAFE_DISPOSE(psBuffer);
}