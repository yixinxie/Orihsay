//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct PixelOutputType
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
	float4 normal : SV_Target2;
	float4 light : SV_Target3;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
PixelOutputType main(PixelInputType input) : SV_TARGET
{
	PixelOutputType output;
	output.diffuse = input.color;
	output.specular = float4(0, 1, 0, 1);
	output.normal = float4(input.normal, 1);
	output.light = float4(0, 1, 1, 1);
	
	return output;
}
