#include "DXInstancing.h"
#include "../misc/CharHelper.h"
DXInstancing::DXInstancing(ID3D11Device *_dev, ID3D11DeviceContext *_devcon) :dev(_dev), devcon(_devcon){

}
void DXInstancing::initShaders(){
	HRESULT hr;
	// shader

	CharBuffer* vsBuffer = CharHelper::loadFile("uiquad_vs.cso");
	dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexShader);

	CharBuffer* psBuffer = CharHelper::loadFile("uiquad_ps.cso");
	dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelShader);

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

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	//D3D11_BUFFER_DESC matrixBufferDesc = {
	//	D3D11_USAGE_DYNAMIC,
	//	sizeof(MatrixBufferStruct),
	//	D3D11_BIND_CONSTANT_BUFFER,
	//	D3D11_CPU_ACCESS_WRITE,
	//	0,
	//	0 };

	//// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//hr = dev->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	//if (FAILED(hr))
	//{
	//}

}
void DXInstancing::init(){
	HRESULT hr;
	// vertex buffer
	D3DXVECTOR3 quadVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f },
		{ 0.45f, -0.5f, 0.0f },
		{ -0.45f, -0.5f, 0.0f },
	};

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(D3DXVECTOR3) * ARRAYSIZE(quadVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { quadVertices, 0, 0 };

	hr = dev->CreateBuffer(&bd, &srd, &vertexbuffer);
	if (FAILED(hr)){

	}
	//// INSTANCE BUFFER
	{
		// Set the number of instances in the array.
		instanceCount = 4;
		instances = new InstanceStruct[instanceCount];

		// Load the instance array with data.
		instances[0].position = D3DXVECTOR3(-1.5f, -1.5f, 5.0f);
		instances[1].position = D3DXVECTOR3(-1.5f, 1.5f, 5.0f);
		instances[2].position = D3DXVECTOR3(1.5f, -1.5f, 5.0f);
		instances[3].position = D3DXVECTOR3(1.5f, 1.5f, 5.0f);

		D3D11_BUFFER_DESC instanceBufferDesc = { sizeof(InstanceStruct) * instanceCount,
			D3D11_USAGE_DYNAMIC, //D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_VERTEX_BUFFER
			D3D11_CPU_ACCESS_WRITE, //0
			0,
			0
		};

		//D3D11_BUFFER_DESC matrixBufferDesc = {
		//	D3D11_USAGE_DYNAMIC,
		//	sizeof(MatrixBufferStruct),
		//	D3D11_BIND_CONSTANT_BUFFER,
		//	D3D11_CPU_ACCESS_WRITE,
		//	0,
		//	0 };
		D3D11_SUBRESOURCE_DATA srd = { instances, 0, 0 };

		// Create the instance buffer.
		hr = dev->CreateBuffer(&instanceBufferDesc, &srd, &instanceBuffer);
		if (FAILED(hr))
		{

		}

	// Release the instance array now that the instance buffer has been created and loaded.

	}

	{
		
	}

}
void DXInstancing::render(){

}
void DXInstancing::dispose(){
	SAFE_RELEASE(vertexbuffer);
	SAFE_RELEASE(instanceBuffer);
}