#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "../math/EasyMath.h"
class DXShadowMap{
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	ID3D11Texture2D* rgbTex;
	ID3D11Texture2D* depthTex;
	D3D11_VIEWPORT viewport;
	
	void initShaders(void);

public:
	ID3D11ShaderResourceView* rgbSRV;
	ID3D11DepthStencilView* depthDSV;
	ID3D11RenderTargetView* rgbRTV;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	// shadow map related
	ID3D11Buffer* lightViewProjMatrixCB;
	ID3D11Buffer* lightSourceViewProjMatrixCB;
	ID3D11Buffer* lightSourcePositionCB;

	ID3D11VertexShader* shadowMapVertexShader;
	ID3D11PixelShader* shadowMapPixelShader;

	ID3D11SamplerState* samplerState;

	DXShadowMap(ID3D11Device* _dev, ID3D11DeviceContext* _devcon);
	void dispose(void);
	void init(int width, int height);
	void setRenderTarget_DepthBuffer(void);
	void updateLightViewCB(const Vector3 position, const Vector3 euler, float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
};