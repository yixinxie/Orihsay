#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#define DEFERRED_G_BUFFER_COUNT 4
class DXDeferred{
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	ID3D11Texture2D* rgbTex[DEFERRED_G_BUFFER_COUNT];
	ID3D11ShaderResourceView* rgbSRV[DEFERRED_G_BUFFER_COUNT];
	ID3D11RenderTargetView* rgbRTV[DEFERRED_G_BUFFER_COUNT];

	ID3D11Texture2D* depthTex;
	ID3D11DepthStencilView* depthDSV;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11VertexShader* combineVertexShader;
	ID3D11PixelShader* combinePixelShader;
	void initDepthStencil(int width, int heightid);
public:
	DXDeferred(ID3D11Device* _dev, ID3D11DeviceContext* _devcon);
	void init(int width, int height);
	void initShaders(void);
	void dispose(void);
};