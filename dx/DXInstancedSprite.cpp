#include "DXInstancedSprite.h"
#include "DXManager.h"
#include "../misc/CharHelper.h"
#define COLOR0 1, 0, 0, 1
#define COLOR1 0, 1, 0, 1
#define COLOR2 0, 0, 1, 1
#define COLOR3 1, 1, 0, 1
#define COLOR4 1, 0, 1, 1
#define COLOR5 0, 1, 1, 1
#define COLOR6 1, 1, 1, 1
#define COLOR7 0.5f, 0.5f, 0.5f, 1
DXInstancedSprite::DXInstancedSprite(ID3D11Device *_dev, ID3D11DeviceContext *_devcon) :dev(_dev), devcon(_devcon){
	inputLayout = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	instanceBuffer = nullptr;

	instanceCount = 0;
	instanceMaxSize = 4;
}
void DXInstancedSprite::init(){
	initShadersAndInputLayout();
	initInstanceBuffer();
}
void DXInstancedSprite::initShadersAndInputLayout(){
	HRESULT hr;
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		
		{ "INSTANCE_MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE_MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
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

void DXInstancedSprite::initInstanceBuffer(){
	HRESULT hr;
	instances = new InstanceStruct[instanceMaxSize];

	// fill the buffer with placeholder values.
	for (int i = 0; i < instanceMaxSize; i++){
		D3DXMatrixScaling(&(instances[i].worldMatrix), 0, 0, 0);
	}

	D3D11_BUFFER_DESC instanceBufferDesc = { 
		sizeof(InstanceStruct) * instanceMaxSize,
		D3D11_USAGE_DYNAMIC, //D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER,//D3D11_BIND_CONSTANT_BUFFER, //
		D3D11_CPU_ACCESS_WRITE,
		0,
		0
	};
	D3D11_SUBRESOURCE_DATA srd = { instances, 0, 0 };

	// Create the instance buffer.
	hr = dev->CreateBuffer(&instanceBufferDesc, &srd, &instanceBuffer);
	if (FAILED(hr))
	{
		TRACE("per instance buffer create failed!");
	}
}
void DXInstancedSprite::updateInstanceBuffer(const std::unordered_map<int, ObjectTransformDesc*>& instancedObjects){
	HRESULT hr;
	int transformCount = instancedObjects.size();
	// instance buffer is refreshed every update.
	if (transformCount > instanceMaxSize){
		// now we need to resize the instance buffer to match the new set of objects.
		instanceMaxSize *= 2;
		SAFE_RELEASE(instanceBuffer);
		initInstanceBuffer();
	}
	
	// after ensuring we have a buffer large enough to hold the object transforms, we copy the transforms to the buffer


	D3D11_MAPPED_SUBRESOURCE resource;
	hr = devcon->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	InstanceStruct* instanceData = (InstanceStruct*)resource.pData;
	int inc = 0;
	for (auto it = instancedObjects.begin(); it != instancedObjects.end(); ++it){
		D3DXMATRIX world, rotationX, rotationY, rotationZ, scale;
		D3DXMatrixTranslation(&world, it->second->position.x, it->second->position.y, it->second->position.z);
		D3DXMatrixRotationX(&rotationX, it->second->rotation.x);
		D3DXMatrixRotationY(&rotationY, it->second->rotation.y);
		D3DXMatrixRotationZ(&rotationZ, it->second->rotation.z);
		D3DXMatrixScaling(&scale, it->second->scale.x, it->second->scale.y, it->second->scale.z);
		instanceData[inc].worldMatrix = scale * (rotationX*rotationY*rotationZ)*world;
		inc++;
	}

	devcon->Unmap(instanceBuffer, 0);

	instanceCount = transformCount;
}
void DXInstancedSprite::render(ID3D11Buffer** viewProjCB){
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(PerVertexData);
	strides[1] = sizeof(InstanceStruct);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;

	devcon->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0); // 1??? questionable!
	devcon->VSSetConstantBuffers(0, 1, viewProjCB);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->IASetInputLayout(inputLayout);
	
	devcon->VSSetShader(vertexShader, NULL, 0);
	
	devcon->PSSetShader(pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	//devcon->PSSetSamplers(0, 1, &m_sampleState);

	devcon->DrawIndexedInstanced(36, instanceCount, 0, 0, 0);
}


void DXInstancedSprite::setVertexAndIndexBuffers(){
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(PerVertexData);
	strides[1] = sizeof(InstanceStruct);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;

	devcon->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}
void DXInstancedSprite::draw(){
	devcon->DrawIndexedInstanced(36, instanceCount, 0, 0, 0);
}
void DXInstancedSprite::setInputLayout(){
	devcon->IASetInputLayout(inputLayout);

}
void DXInstancedSprite::dispose(){
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(instanceBuffer);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(inputLayout);
}