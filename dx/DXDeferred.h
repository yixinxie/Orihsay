#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#define DEFERRED_G_BUFFER_COUNT 4
#define DEFERRED_G_BUFFER_DIFFUSE 0
#define DEFERRED_G_BUFFER_SPEC 1
#define DEFERRED_G_BUFFER_NORMAL 2
#define DEFERRED_G_BUFFER_LIGHT 3
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

	ID3D11InputLayout* inputLayout;
	ID3D11InputLayout* quadInputLayout;
	void initDepthStencil(int width, int height);
	void initGBuffers(int width, int height);
public:
	DXDeferred(ID3D11Device* _dev, ID3D11DeviceContext* _devcon);
	void init(int width, int height);
	void initShaders(void);
	void dispose(void);

	// g buffer pass
	void setRenderTargets_GBuffers(void);
	void setShaders(void);
	void setInputLayout_GBuffers(void);
	void setShaderConstants(ID3D11Buffer** viewProjCB);
	// combine pass
	void setInputLayout_Shading(void);
	void setShaders_Shading(void);
	void setShaderConstants_Shading(ID3D11Buffer** viewProjCB, ID3D11SamplerState* sampler);
	void postDraw(void);
};