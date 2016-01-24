#include "DXInstancing.h"
#include "directx.h"
#include "../misc/CharHelper.h"
DXInstancing::DXInstancing(ID3D11Device *_dev, ID3D11DeviceContext *_devcon) :dev(_dev), devcon(_devcon){
	inputLayout = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
}
void DXInstancing::initShaders(){
	HRESULT hr;
	// shader

	CharBuffer* vsBuffer = CharHelper::loadFile("instanced_quad_vs.cso");
	hr = dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);
	if (FAILED(hr)){
		TRACE("vertex shader create failed");
	}

	CharBuffer* psBuffer = CharHelper::loadFile("instanced_quad_ps.cso");
	hr = dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);
	if (FAILED(hr)){
		TRACE("pixel shader create failed");
	}

	/*delete vsBuffer->buffer;
	delete vsBuffer;

	delete psBuffer->buffer;
	delete psBuffer;*/

	devcon->VSSetShader(vertexShader, nullptr, 0);
	devcon->PSSetShader(pixelShader, nullptr, 0);

	
	
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	/*
	For instancing we are going to add a third element to the layout.As we plan to position the four instanced triangles in different positions in 3D space we need to send the shader 
	an extra position vector composed of three floats representing x, y, and z.Therefore the format is set to DXGI_FORMAT_R32G32B32_FLOAT.The semantic name is set to TEXCOORD as we are just using 
	a generic semantic.Note that we have to set the semantic index to 1 in the layout since there is already a TEXCOORD for the texture coordinates using slot 0.

	Now for the instancing specific stuff we set the InputSlotClass to D3D11_INPUT_PER_INSTANCE_DATA which indicates that this is instanced data.Secondly the InstanceDataStepRate is now used and 
	we set the step rate to 1 so that it will draw one instance before stepping forward a unit in the instance data.Note also that this is the first unit in the instance buffer 
	so we set the AlignedByteOffset to 0 again since we are not aligning to the vertex buffer with the instance data.
	*/

	// Get a count of the elements in the layout.
	int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	hr = dev->CreateInputLayout(polygonLayout, numElements, vsBuffer->buffer, vsBuffer->length,
		&inputLayout);
	if (FAILED(hr)){
		TRACE("input layout create error!");
	}
}
void DXInstancing::init(){
	HRESULT hr;
	// vertex buffer
	PerVertexData quadVertices[] =
	{
		{ { 0.0f, 0.5f, 0.0f } },
		{ {0.45f, -0.5f, 0.0f } },
		{ {-0.45f, -0.5f, 0.0f } },
	};

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(PerVertexData) * ARRAYSIZE(quadVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { quadVertices, 0, 0 };

	hr = dev->CreateBuffer(&bd, &srd, &vertexBuffer);
	if (FAILED(hr)){
		TRACE("vertex buffer create failed!");
	}
	//// INSTANCE BUFFER
	{
		// Set the number of instances in the array.
		instanceCount = 4;
		instances = new InstanceStruct[instanceCount];

		// Load the instance array with data.
		instances[0].position = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
		instances[1].position = D3DXVECTOR3(-0.1f, 0.1f, 0.1f);
		instances[2].position = D3DXVECTOR3(-0.1f, -0.1f, 0.1f);
		instances[3].position = D3DXVECTOR3(0.1f, -0.1f, 0.1f);

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
void DXInstancing::render(){
	// Set the vertex input layout.
	
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];
	/*
	The instance buffer is just a second vertex buffer containing different information so it is set on the device at the same time using the same call as the vertex buffer.So instead of how we previously sent in a single stride, offset, and buffer we now send an array of strides, offsets, and buffers to the IASetVertexBuffers call.
	First we set the two strides to the size of the VertexType and InstanceType.*/

	// Set the buffer strides.
	strides[0] = sizeof(PerVertexData);
	strides[1] = sizeof(InstanceStruct);

	//We then set the offsets for both the vertex and instance buffer.

	// Set the buffer offsets.
	offsets[0] = 0;
	offsets[1] = 0;
	//Next we create an array that holds the pointers to the vertex buffer and the instance buffer.

	// Set the array of pointers to the vertex and instance buffers.
	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;
	//Finally we set both the vertex buffer and the instance buffer on the device context in the same call.

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	devcon->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->IASetInputLayout(inputLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	devcon->VSSetShader(vertexShader, NULL, 0);
	devcon->PSSetShader(pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	//devcon->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	devcon->DrawInstanced(3, instanceCount, 0, 0);
}
void DXInstancing::dispose(){
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(instanceBuffer);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(inputLayout);

}