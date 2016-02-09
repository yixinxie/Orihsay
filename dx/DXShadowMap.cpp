#include "DXShadowMap.h"
#include "../misc/Macros.h"
#include "../misc/CharHelper.h"
#include "../math/EasyMath.h"
#include "../gameplay/Transform.h" //???
#include "DXShaderStructs.h"
DXShadowMap::DXShadowMap(ID3D11Device* _dev, ID3D11DeviceContext* _devcon) :
rgbTex(nullptr), rgbRTV(nullptr), rgbSRV(nullptr), depthTex(nullptr), depthDSV(nullptr) {
	dev = _dev;
	devcon = _devcon;
	shadowMapVertexShader = nullptr;
	shadowMapPixelShader = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	samplerState = nullptr;

	lightViewProjMatrixCB = nullptr;
	lightSourceViewProjMatrixCB = nullptr;
	lightSourcePositionCB = nullptr;
}
void DXShadowMap::dispose(){
	SAFE_RELEASE(rgbTex);
	SAFE_RELEASE(rgbRTV);
	SAFE_RELEASE(rgbSRV);
	SAFE_RELEASE(depthTex);
	SAFE_RELEASE(depthDSV);

	SAFE_RELEASE(shadowMapVertexShader);
	SAFE_RELEASE(shadowMapPixelShader);

	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);

	SAFE_RELEASE(samplerState);

	SAFE_RELEASE(lightViewProjMatrixCB);
	SAFE_RELEASE(lightSourceViewProjMatrixCB);
	SAFE_RELEASE(lightSourcePositionCB);

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

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = dev->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr))
	{
		TRACE("depth map texture sampler state create failed.");
	}
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
void DXShadowMap::setRenderTarget_DepthBuffer(){
	ID3D11ShaderResourceView* emptySRV[1] = { nullptr };
	devcon->PSSetShaderResources(0, 1, emptySRV);
	devcon->OMSetRenderTargets(1, &rgbRTV, depthDSV);

	devcon->ClearRenderTargetView(rgbRTV, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	devcon->ClearDepthStencilView(depthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DXShadowMap::updateLightViewCB(const Vector3 position, const Vector3 euler, float fieldOfView, float aspectRatio, float nearPlane, float farPlane){
	Vector3 _lookat, _up;

	Transform::getLookatAndUp(euler, &_lookat, &_up);

	D3DXVECTOR3 eye(position.x, position.y, position.z);
	D3DXVECTOR3 lookAt(position.x + _lookat.x, position.y + _lookat.y, position.z + _lookat.z);
	D3DXVECTOR3 up(_up.x, _up.y, _up.z);

	ViewProjection wvp;
	D3DXMatrixLookAtLH(&(wvp.view), &eye, &lookAt, &up);
	D3DXMatrixPerspectiveFovLH(&(wvp.projection), D3DXToRadian(fieldOfView), aspectRatio, nearPlane, farPlane);
	HRESULT hr;
	if (lightSourceViewProjMatrixCB == nullptr){
		D3DXMatrixTranspose(&(wvp.view), &(wvp.view));
		D3DXMatrixTranspose(&(wvp.projection), &(wvp.projection));
		D3D11_BUFFER_DESC cbd = { sizeof(wvp), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0 };
		D3D11_SUBRESOURCE_DATA cbdInitData = { &wvp, 0, 0 };

		hr = dev->CreateBuffer(&cbd, &cbdInitData, &lightSourceViewProjMatrixCB);
		if (FAILED(hr)){
			TRACE("camera constant buffer create failed!");
		}
	}
	else {
		// if the view matrix buffer is already created, we just need to update it.
		D3D11_MAPPED_SUBRESOURCE resource;
		hr = devcon->Map(lightSourceViewProjMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		ViewProjection* viewProjectionMatrices = (ViewProjection*)resource.pData;
		D3DXMatrixTranspose(&(viewProjectionMatrices->view), &(wvp.view));
		D3DXMatrixTranspose(&(viewProjectionMatrices->projection), &(wvp.projection));

		devcon->Unmap(lightSourceViewProjMatrixCB, 0);

	}
	if (lightSourcePositionCB == nullptr){
		D3D11_BUFFER_DESC cbd = { sizeof(Vector4), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0 };
		Vector4 tmp = Vector4(position);

		D3D11_SUBRESOURCE_DATA cbdInitData = { &tmp, 0, 0 };

		hr = dev->CreateBuffer(&cbd, &cbdInitData, &lightSourcePositionCB);
		if (FAILED(hr)){
			TRACE("camera constant buffer create failed!");
		}
	}
	else{
		// if the view matrix buffer is already created, we just need to update it.
		D3D11_MAPPED_SUBRESOURCE resource;
		hr = devcon->Map(lightSourcePositionCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		Vector3* lightPos = (Vector3*)resource.pData;
		*lightPos = position;

		devcon->Unmap(lightSourcePositionCB, 0);
	}
}