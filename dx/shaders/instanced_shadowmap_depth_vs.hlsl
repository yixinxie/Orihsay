
struct VertexInputType
{
    float3 position : POSITION;

	// during the first pass of shadow map, when rendering just the depth values to an RGB buffer,
	// additional data per vertex data such as color, texture coord are not needed.
	// we could optimize this by not passing the additional stuff to the vertex shader by
	// creating another vertex buffer that only holds the position data. 
	float4 color : COLOR;
	// although for instanced drawing with the shadow map technique, the world matrix is still required.
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
    float4 depthPosition : TEXTURE0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    
	// Change the position vector to be 4 units for proper matrix calculations.
	matrix worldMatrix = float4x4(input.worldMatrix0, input.worldMatrix1, input.worldMatrix2, input.worldMatrix3);
    output.position.w = 1.0f;
	output.position.xyz = input.position;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the position value in a second input value for depth value calculations.
	output.depthPosition = output.position;
	
	return output;
}