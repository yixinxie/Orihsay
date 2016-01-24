#include "directx.h"
DirectX11::DirectX11(void){
}
void DirectX11::init(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);


	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target--------set once per camera
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer -----------camera dependent
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);


	// Set the viewport -----------------camera dependent
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
}
void DirectX11::close()
{
	// close and release all existing COM objects
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}


void DirectX11::renderFrame(void)
{
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	//devcon->OMSetRenderTargets(1, &backbuffer, nullptr);

	// set the vertex buffer
	UINT stride = sizeof(D3DXVECTOR3);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);

	// set the primitive topology
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw 3 vertices, starting from vertex 0
	devcon->Draw(3, 0);

	// switch the back buffer and the front buffer
	swapchain->Present(1, 0);
	//swapchain->Present(0, 0);
}
void DirectX11::initQuadBuffer(void){
	// vertex buffer
	D3DXVECTOR3 OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f },
		{ 0.45f, -0.5f, 0.0f },
		{ -0.45f, -0.5f, 0.0f },
	};

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(D3DXVECTOR3) * ARRAYSIZE(OurVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	

	D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };
	dev->CreateBuffer(&bd, &srd, &vertexbuffer);

	// shader
	CharBuffer* vsBuffer = CharHelper::loadFile("uiquad_vs.cso");
	dev->CreateVertexShader(vsBuffer->buffer, vsBuffer->length, nullptr, &vertexshader);

	CharBuffer* psBuffer = CharHelper::loadFile("uiquad_ps.cso");
	dev->CreatePixelShader(psBuffer->buffer, psBuffer->length, nullptr, &pixelshader);

	/*delete vsBuffer->buffer;
	delete vsBuffer;

	delete psBuffer->buffer;
	delete psBuffer;*/

	devcon->VSSetShader(vertexshader, nullptr, 0);
	devcon->PSSetShader(pixelshader, nullptr, 0);

	// input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create and set the input layout
	dev->CreateInputLayout(ied, ARRAYSIZE(ied), vsBuffer->buffer, vsBuffer->length, &inputlayout);
	devcon->IASetInputLayout(inputlayout);

}
void DirectX11::disposeQuadBuffer(){
	SAFE_RELEASE(vertexshader);
	SAFE_RELEASE(pixelshader);
	SAFE_RELEASE(vertexbuffer);
	SAFE_RELEASE(inputlayout);
}


void DirectX11::initInstancing(){
	instancedDraw = new DXInstancing(dev, devcon);
	instancedDraw->init();
	instancedDraw->initShaders();

}
void DirectX11::disposeInstancing(){
	SAFE_DISPOSE(instancedDraw);
}
void DirectX11::renderWithInstancing(){
	HRESULT hr;
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	instancedDraw->render();
	hr = swapchain->Present(1, 0);
	if (FAILED(hr)){
		TRACE("present failed!");
	}
}