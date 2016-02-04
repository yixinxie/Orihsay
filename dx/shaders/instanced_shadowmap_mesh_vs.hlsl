
struct VertexInputType
{
	float3 position : POSITION;
	float4 color : COLOR;
	float4 worldMatrix0 : INSTANCE_MATRIX0;
	float4 worldMatrix1 : INSTANCE_MATRIX1;
	float4 worldMatrix2 : INSTANCE_MATRIX2;
	float4 worldMatrix3 : INSTANCE_MATRIX3;
};
cbuffer VSHADER_CB
{
	matrix viewMatrix;
	matrix projectionMatrix;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	matrix world = float4x4(input.worldMatrix0, input.worldMatrix1, input.worldMatrix2, input.worldMatrix3);
	output.position.w = 1.0f;
	output.position.xyz = input.position;
	output.position = mul(output.position, world);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = input.color;

	//output.position = input.position;


	return output;
}