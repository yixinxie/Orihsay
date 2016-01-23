#include "directx.h"
class DXInstancing{
private:
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;

	ID3D11InputLayout* inputLayout;
	

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11Buffer* instanceBuffer;
	ID3D11Buffer* vertexbuffer;
	int instanceCount;
	struct InstanceStruct{
		D3DXVECTOR3 position;
	};
	InstanceStruct* instances;

	struct MatrixBufferStruct
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
	
	MatrixBufferStruct matrixBuffer;
public:
	DXInstancing(ID3D11Device *_dev, ID3D11DeviceContext *_devcon);
	void init(void);
	void initShaders(void);
	void render(void);
	void dispose(void);
};