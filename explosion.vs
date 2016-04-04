//vertex shader

//globals

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ExplosionBuffer
{
    float3 distortion;
    float time;
};

Texture2D shaderTexture;
SamplerState SampleType;


//structs

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 deltaP : DELTAPOS;
};

//Vertex shader

PixelInputType ExplosionVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	uint3 sampleCoord = uint3(256*input.normal.x + 256, 256*input.normal.y + 256, 0);
	float4 distex = shaderTexture.Load(sampleCoord);
	
	
	
	float4 finalDis;
	finalDis.x = distortion.x * distex.r;
	finalDis.y = distortion.y * distex.g;
	finalDis.z = distortion.z * distex.b;
	finalDis.w = 0.0f;

	input.position = input.position + finalDis;

	float norm = dot(input.position, input.position) / 8;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the input color for the pixel shader to use.
    output.tex = input.tex;
	
	output.normal = input.normal;

	output.deltaP.x = norm;
    

    return output;
}