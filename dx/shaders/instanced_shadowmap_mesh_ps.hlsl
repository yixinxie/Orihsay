//Texture2D shaderTexture : register(t0);
Texture2D depthMapTex : register(t0);


SamplerState samplerClamp : register(s0);
//SamplerState samplerWrap  : register(s1);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR; // vertex color
	float4 lightViewPosition : TEXCOORD0;
	float3 lightDir : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	float4 outputColor = input.color;
	float2 projCoord;
	projCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	if (projCoord.x >= 0.0f && projCoord.x <= 1.0f && projCoord.y >= 0.0f && projCoord.y <= 1.0f){
		// inside the shadowmap
		float depthValue = depthMapTex.Sample(samplerClamp, projCoord).r;
		float lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		if (lightDepthValue - 0.00001f > depthValue){
			outputColor *= 0.5f;
		}

	}

	return outputColor;
}
