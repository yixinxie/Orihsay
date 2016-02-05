
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
cbuffer Light_CB{
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
}
cbuffer Light_CB2{
	float4 lightPosition;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR; // vertex color
	float4 lightViewPosition : TEXCOORD0;
	float3 lightDir : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	matrix worldMatrix = float4x4(input.worldMatrix0, input.worldMatrix1, input.worldMatrix2, input.worldMatrix3);
	output.position.w = 1.0f;
	output.position.xyz = input.position;
	output.position = mul(output.position, worldMatrix);

	float4 worldPos = output.position;// store the world position for later use

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// calculate the position in view space
	output.lightViewPosition = mul(worldPos, lightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	output.color = input.color;

	output.lightDir = normalize(lightPosition.xyz - worldPos.xyz);

	return output;
}