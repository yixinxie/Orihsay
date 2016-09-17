#include "DXInstancedSprite.h"
#include "DXManager.h"
#include "../misc/CharHelper.h"
DXInstancedSprite::DXInstancedSprite(ID3D11Device *_dev, ID3D11DeviceContext *_devcon) :dev(_dev), devcon(_devcon){
	inputLayout = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	vertexBuffer = nullptr;
	//indexBuffer = nullptr;
	//instanceBuffer = nullptr;
	samplerState = nullptr;
	alphaBlendState = nullptr;

	instanceCount = 0;
	instanceMaxSize = 4;
}
void DXInstancedSprite::init(){
	initShadersAndInputLayout();
	initVertexBuffer();
	//initInstanceBuffer();
	initBlendState();
}
void DXInstancedSprite::initBlendState(void){
	D3D11_BLEND_DESC omDesc;
	ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
	omDesc.RenderTarget[0].BlendEnable = true;
	omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	dev->CreateBlendState(&omDesc, &alphaBlendState);

}
void DXInstancedSprite::initShadersAndInputLayout(){
	HRESULT hr;

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
	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MaxLOD = 0;

	// Create the texture sampler state.
	hr = dev->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr))
	{
		TRACE("depth map texture sampler state create failed.");
	}

	// shader

	CharBuffer* vsBuffer = CharHelper::loadFile("instanced_sprite_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);
	if (FAILED(hr)){
		TRACE("vertex shader create failed");
	}

	CharBuffer* psBuffer = CharHelper::loadFile("instanced_sprite_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);
	if (FAILED(hr)){
		TRACE("pixel shader create failed");
	}
	
	// Create the vertex input layout description.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE_UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		//{ "INSTANCE_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//{ "INSTANCE_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//{ "INSTANCE_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		//{ "INSTANCE_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	// Get a count of the elements in the layout.
	int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	hr = dev->CreateInputLayout(polygonLayout, numElements, vsBuffer->buffer, vsBuffer->length,
		&inputLayout);
	if (FAILED(hr)){
		TRACE("input layout create error!");
	}

	delete vsBuffer->buffer;
	delete vsBuffer;

	delete psBuffer->buffer;
	delete psBuffer;
}
// the vertex positions can change quite frequently when doing ui motions.
void DXInstancedSprite::initVertexBuffer(){
	HRESULT hr;
	// vertex buffer: the vertex buffer stores the vertices of a quad and it is created only once per execution.
	int vertexCount = instanceMaxSize * 6;
	SpriteVertexData* quadVertices = new SpriteVertexData[vertexCount];
	int bufferSize = sizeof(SpriteVertexData) * vertexCount;
	ZeroMemory(quadVertices, bufferSize);

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA srd = { quadVertices, 0, 0 };

	hr = dev->CreateBuffer(&bd, &srd, &vertexBuffer);
	if (FAILED(hr)){
		TRACE("vertex buffer create failed!");
	}
	delete quadVertices;
	//// index buffer
	//{
	//	unsigned int indices[] = {
	//		0, 3, 1, 0, 2, 3
	//	};

	//	D3D11_BUFFER_DESC bufferDesc = {
	//		sizeof(indices),
	//		D3D11_USAGE_DEFAULT,
	//		D3D11_BIND_INDEX_BUFFER,
	//		0,
	//		0,
	//		0
	//	};

	//	// Define the resource data.
	//	D3D11_SUBRESOURCE_DATA InitData = { indices, 0, 0 };

	//	// Create the buffer with the device.
	//	hr = dev->CreateBuffer(&bufferDesc, &InitData, &indexBuffer);
	//	if (FAILED(hr)){
	//		TRACE("index buffer create failed!");
	//	}
	//}
}
//void DXInstancedSprite::initInstanceBuffer(){
//	HRESULT hr;
//	instances = new InstanceStruct[instanceMaxSize];
//
//	// fill the buffer with placeholder values.
//	for (int i = 0; i < instanceMaxSize; i++){
//		D3DXMatrixScaling(&(instances[i].worldMatrix), 0, 0, 0);
//	}
//
//	D3D11_BUFFER_DESC instanceBufferDesc = { 
//		sizeof(InstanceStruct) * instanceMaxSize,
//		D3D11_USAGE_DYNAMIC, //D3D11_USAGE_DEFAULT,
//		D3D11_BIND_VERTEX_BUFFER,//D3D11_BIND_CONSTANT_BUFFER, //
//		D3D11_CPU_ACCESS_WRITE,
//		0,
//		0
//	};
//	D3D11_SUBRESOURCE_DATA srd = { instances, 0, 0 };
//
//	// Create the instance buffer.
//	hr = dev->CreateBuffer(&instanceBufferDesc, &srd, &instanceBuffer);
//	if (FAILED(hr))
//	{
//		TRACE("per instance buffer create failed!");
//	}
//}
void DXInstancedSprite::updateInstanceBuffer(const std::unordered_map<int, ObjectRectTransformDesc*>& sprites){
	HRESULT hr;
	int transformCount = sprites.size();
	// instance buffer is refreshed every update.
	if (transformCount > instanceMaxSize){
		// now we need to resize the instance buffer to match the new set of objects.
		instanceMaxSize *= 2;
		SAFE_RELEASE(vertexBuffer);
		initVertexBuffer();
	}
	
	// after ensuring we have a buffer large enough to hold the object transforms, we copy the transforms to the buffer

	D3D11_MAPPED_SUBRESOURCE resource;
	hr = devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	SpriteVertexData* vertexData = (SpriteVertexData*)resource.pData;
	int inc = 0;
	for (auto it = sprites.begin(); it != sprites.end(); ++it){
		ObjectRectTransformDesc* transformDesc = it->second;
		vertexData[inc].position.x = transformDesc->position.x - transformDesc->widthHeight.x;
		vertexData[inc].position.y = transformDesc->position.y - transformDesc->widthHeight.y;
		vertexData[inc].position.z = 0;
		vertexData[inc].uv = Vector2(0, 1);

		//vertexData[inc].position.z = vertexData[inc].position.y;
		inc++;

		vertexData[inc].position.x = transformDesc->position.x - transformDesc->widthHeight.x;
		vertexData[inc].position.y = transformDesc->position.y + transformDesc->widthHeight.y;
		vertexData[inc].position.z = 0;
		//vertexData[inc].position.z = vertexData[inc].position.y;
		vertexData[inc].uv = Vector2(0, 0);
		inc++;

		vertexData[inc].position.x = transformDesc->position.x + transformDesc->widthHeight.x;
		vertexData[inc].position.y = transformDesc->position.y + transformDesc->widthHeight.y;
		vertexData[inc].position.z = 0;
		//vertexData[inc].position.z = vertexData[inc].position.y;
		vertexData[inc].uv = Vector2(1, 0);
		inc++;

		vertexData[inc].position.x = transformDesc->position.x - transformDesc->widthHeight.x;
		vertexData[inc].position.y = transformDesc->position.y - transformDesc->widthHeight.y;
		vertexData[inc].position.z = 0;
		//vertexData[inc].position.z = vertexData[inc].position.y;
		vertexData[inc].uv = Vector2(0, 1);
		inc++;

		vertexData[inc].position.x = transformDesc->position.x + transformDesc->widthHeight.x;
		vertexData[inc].position.y = transformDesc->position.y + transformDesc->widthHeight.y;
		vertexData[inc].position.z = 0;
		vertexData[inc].uv = Vector2(1, 0);
		//vertexData[inc].position.z = vertexData[inc].position.y;
		inc++;

		vertexData[inc].position.x = transformDesc->position.x + transformDesc->widthHeight.x;
		vertexData[inc].position.y = transformDesc->position.y - transformDesc->widthHeight.x;
		vertexData[inc].position.z = 0;
		vertexData[inc].uv = Vector2(1, 1);
		//vertexData[inc].position.z = vertexData[inc].position.y;
		inc++;
	}

	devcon->Unmap(vertexBuffer, 0);

	instanceCount = transformCount;
}
void DXInstancedSprite::render(ID3D11Buffer** viewProjCB, ID3D11ShaderResourceView **shaderRV){
	unsigned int strides[1];
	unsigned int offsets[1];
	ID3D11Buffer* bufferPointers[1];

	strides[0] = sizeof(SpriteVertexData);
	//strides[1] = sizeof(InstanceStruct);

	offsets[0] = 0;
	//offsets[1] = 0;

	bufferPointers[0] = vertexBuffer;
	//bufferPointers[1] = instanceBuffer;

	devcon->IASetVertexBuffers(0, 1, bufferPointers, strides, offsets);
	//devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0); // 1??? questionable!
	devcon->VSSetConstantBuffers(0, 1, viewProjCB);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->IASetInputLayout(inputLayout);
	
	devcon->VSSetShader(vertexShader, NULL, 0);
	
	devcon->PSSetShader(pixelShader, NULL, 0);

	devcon->PSSetShaderResources(0, 1, shaderRV);
	//ID3D11SamplerState* samplers[DEFERRED_G_BUFFER_COUNT];

	devcon->PSSetSamplers(0, 1, &samplerState);

	// Set the sampler state in the pixel shader.
	//devcon->PSSetSamplers(0, 1, &m_sampleState);

	//devcon->DrawIndexedInstanced(36, instanceCount, 0, 0, 0);
	devcon->OMSetBlendState(alphaBlendState, nullptr, 0xffffffff);
	devcon->Draw(instanceCount*6, 0);
}


//void DXInstancedSprite::setVertexAndIndexBuffers(){
//	unsigned int strides[2];
//	unsigned int offsets[2];
//	ID3D11Buffer* bufferPointers[2];
//
//	strides[0] = sizeof(SpriteVertexData);
//
//
//	offsets[0] = 0;
//	offsets[1] = 0;
//
//	bufferPointers[0] = vertexBuffer;
//	bufferPointers[1] = instanceBuffer;
//
//	devcon->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
//	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//}
//void DXInstancedSprite::draw(){
//	devcon->DrawIndexedInstanced(36, instanceCount, 0, 0, 0);
//}
//void DXInstancedSprite::setInputLayout(){
//	devcon->IASetInputLayout(inputLayout);
//
//}
void DXInstancedSprite::dispose(){
	SAFE_RELEASE(alphaBlendState);
	SAFE_RELEASE(samplerState);
	SAFE_RELEASE(vertexBuffer);
	//SAFE_RELEASE(indexBuffer);
	//SAFE_RELEASE(instanceBuffer);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(inputLayout);
}