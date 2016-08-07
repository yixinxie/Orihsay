#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

using namespace std;
namespace OriGraphics{
	struct DXTextureResource{
		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* textureSRV;

	};
}