#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
class DXShadowMap{
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	ID3D11Texture2D* rgbTex;
	
	ID3D11ShaderResourceView* rgbSRV;

	ID3D11Texture2D* depthTex;
	

	D3D11_VIEWPORT viewport;
	
	void initShaders(void);
public:
	ID3D11DepthStencilView* depthDSV;
	ID3D11RenderTargetView* rgbRTV;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11VertexShader* shadowMapVertexShader;
	ID3D11PixelShader* shadowMapPixelShader;

	DXShadowMap(ID3D11Device* _dev, ID3D11DeviceContext* _devcon);
	void dispose(void);
	void init(int width, int height);
	void prepareLightView(void);
};