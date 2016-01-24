//float4 main(float4 pos : POSITION) : SV_POSITION
//{
//	return pos;
//}

//////////////////
struct VertexInputType
{
	float4 position : POSITION;

	float3 instancePosition : TEXCOORD1;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	//Here is where we use the instanced position information to modify the position of each triangle we are drawing.

		// Update the position of the vertices based on the data for this particular instance.
	input.position.x += input.instancePosition.x;
	input.position.y += input.instancePosition.y;
	input.position.z += input.instancePosition.z;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	/*output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);*/
	output.position = input.position;


	return output;
}