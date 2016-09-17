Texture2D spriteTex : register(t0);
SamplerState samplerClamp : register(s0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	
	float4 output = spriteTex.Sample(samplerClamp, input.uv);
	//output.a = 0.5f;
	//return input.color;
	return output;
}
