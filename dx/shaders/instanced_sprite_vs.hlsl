//////////////////


struct VertexInputType
{
	float3 position : POSITION;
	float2 uv : TEXTURE_UV;
	//float3 normal : NORMAL;
	//matrix instanceMatrix : INSTANCE_MATRIX;
};
cbuffer VSHADER_CB
{
	//matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR;
};

float4x4 CreateMatrixFromCols(float4 c0, float4 c1, float4 c2, float4 c3) {
	return float4x4(c0.x, c1.x, c2.x, c3.x,
		c0.y, c1.y, c2.y, c3.y,
		c0.z, c1.z, c2.z, c3.z,
		c0.w, c1.w, c2.w, c3.w);
}
////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.

	//matrix world = float4x4(input.worldMatrix0, input.worldMatrix1, input.worldMatrix2, input.worldMatrix3);
	output.position.w = 1.0f;
	output.position.xyz = input.position;
	//output.position = mul(output.position, world);
	//output.position = mul(output.position, viewMatrix);
	//output.position = mul(output.position, projectionMatrix);
	
	output.uv = input.uv;
	output.color = float4(1,1,1,1);
	

	//output.position = input.position;


	return output;
}