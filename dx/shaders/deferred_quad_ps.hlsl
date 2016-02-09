Texture2D diffuseTex : register(t0);
Texture2D specTex : register(t1);
Texture2D normalTex : register(t2);
Texture2D lightingTex : register(t3);

SamplerState diffuseSampler : register(s0);
SamplerState specSampler : register(s1);
SamplerState normalSampler : register(s2);
SamplerState lightingSampler : register(s3);
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
	//return float4(diffuseTex.Sample(diffuseSampler, input.tex).rgb, 1);
	return float4(1.0f, 0, 0, 1.0f);
}
