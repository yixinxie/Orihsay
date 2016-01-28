#include "DXInstancedMesh.h"
#include "directx.h"
#include "../misc/CharHelper.h"
DXInstancedMesh::DXInstancedMesh(ID3D11Device *_dev, ID3D11DeviceContext *_devcon) :dev(_dev), devcon(_devcon){
	inputLayout = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	instanceBuffer = nullptr;
}
void DXInstancedMesh::init(){
	initCubeBuffer();
	initShadersAndInputLayout();
	
}
void DXInstancedMesh::initShadersAndInputLayout(){
	HRESULT hr;
	// shader

	CharBuffer* vsBuffer = CharHelper::loadFile("instanced_simple_mesh_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);
	if (FAILED(hr)){
		TRACE("vertex shader create failed");
	}

	CharBuffer* psBuffer = CharHelper::loadFile("instanced_simple_mesh_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);
	if (FAILED(hr)){
		TRACE("pixel shader create failed");
	}
	
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
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

void DXInstancedMesh::initCubeBuffer(){
	HRESULT hr;
	// vertex buffer: the vertex buffer stores the vertices of a quad and it is created only once per execution.
	PerVertexData quadVertices[] =
	{
		// clockwise triangle winding.
		{ { 0, 0, 0 }, { 0, 1, 0, 1 } },
		{ { 1, 0, 0 }, { 0, 1, 0, 1 } },
		{ { 1, 1, 0 }, { 0, 1, 0, 1 } },
		{ { 0, 1, 0 }, { 0, 1, 0, 1 } },

		{ { 0, 0, 1 }, { 0, 1, 0, 1 } },
		{ { 1, 0, 1 }, { 0, 1, 0, 1 } },
		{ { 1, 1, 1 }, { 0, 1, 0, 1 } },
		{ { 0, 1, 1 }, { 0, 1, 0, 1 } },

		

	};

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(PerVertexData) * ARRAYSIZE(quadVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { quadVertices, 0, 0 };

	hr = dev->CreateBuffer(&bd, &srd, &vertexBuffer);
	if (FAILED(hr)){
		TRACE("vertex buffer create failed!");
	}
	// index buffer
	{
		unsigned int indices[] = { 
			//0, 1, 2, 
			0, 2, 1,
			0, 3, 2,

			1, 2, 6,
			6, 5, 1,

			4, 5, 6,
			6, 7, 4,

			2, 3, 6,
			6, 3, 7,

			0, 7, 3,
			0, 4, 7,

			0, 1, 5,
			0, 5, 4
		};

		// Fill in a buffer description.
		D3D11_BUFFER_DESC bufferDesc = {
			sizeof(unsigned int) * 36,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA InitData = { indices , 0, 0};

		// Create the buffer with the device.
		hr = dev->CreateBuffer(&bufferDesc, &InitData, &indexBuffer);
		if (FAILED(hr)){
			TRACE("index buffer create failed!");
		}
	}
	//// INSTANCE BUFFER
	// instance buffer is refreshed every update.
	{
		// Set the number of instances in the array.
		instanceCount = 4;
		instances = new InstanceStruct[instanceCount];

		// Load the instance array with data.
		/*instances[0].position = D3DXVECTOR3(0.3f, 0.3f, 0);
		instances[1].position = D3DXVECTOR3(-0.3f, 0.3f, 0);
		instances[2].position = D3DXVECTOR3(-0.3f, -0.3f, 0);
		instances[3].position = D3DXVECTOR3(0.3f, -0.3f, 0);*/

		instances[0].position = D3DXVECTOR3(0, 0, 5);
		instances[1].position = D3DXVECTOR3(-1, 0, 5);
		instances[2].position = D3DXVECTOR3(0, -1, 5);
		instances[3].position = D3DXVECTOR3(-1, -1, 5);

		D3D11_BUFFER_DESC instanceBufferDesc = { sizeof(InstanceStruct) * instanceCount,
			D3D11_USAGE_DYNAMIC, //D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_VERTEX_BUFFER
			D3D11_CPU_ACCESS_WRITE, //0
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
}
void DXInstancedMesh::updateInstanceBuffer(){
	HRESULT hr;
	instanceCount = 4;
	instances = new InstanceStruct[instanceCount];

	// Load the instance array with data.
	instances[0].position = D3DXVECTOR3(0.3f, 0.3f, 0);
	instances[1].position = D3DXVECTOR3(-0.3f, 0.3f, 0);
	instances[2].position = D3DXVECTOR3(-0.3f, -0.3f, 0);
	instances[3].position = D3DXVECTOR3(0.3f, -0.3f, 0);

	D3D11_BUFFER_DESC instanceBufferDesc = { sizeof(InstanceStruct) * instanceCount,
		D3D11_USAGE_DYNAMIC, //D3D11_USAGE_DEFAULT,
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_VERTEX_BUFFER
		D3D11_CPU_ACCESS_WRITE, //0
		0,
		0
	};
	D3D11_SUBRESOURCE_DATA srd = { instances, 0, 0 };

	// Create the instance buffer.
	hr = dev->CreateBuffer(&instanceBufferDesc, &srd, &instanceBuffer);
	if (FAILED(hr))
	{
		TRACE("per instance buffer update failed!");
	}
}
void DXInstancedMesh::render(ID3D11Buffer** viewProjCB){
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
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 1);
	devcon->VSSetConstantBuffers(0, 1, viewProjCB);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->IASetInputLayout(inputLayout);
	
	devcon->VSSetShader(vertexShader, NULL, 0);
	
	devcon->PSSetShader(pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	//devcon->PSSetSamplers(0, 1, &m_sampleState);

	devcon->DrawIndexedInstanced(36, instanceCount, 0, 0, 0);
	//devcon->DrawInstanced(6, instanceCount, 0, 0);
}
void DXInstancedMesh::dispose(){
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(instanceBuffer);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(inputLayout);
}