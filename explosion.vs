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
    

	//sample the noise texture 
	//(sampling using normals, which should have copys of texture coords that are same for vetices on edge of uv unwrap)
	uint3 sampleCoord = uint3(512*input.normal.x, 512*input.normal.y, 0);
	float4 distex = shaderTexture.Load(sampleCoord);
	

	//calculate the displacement based on noise tex and distortion buffer
	float displacement;
	displacement = distortion.x * distex.r + distortion.y * distex.g + distortion.z * distex.b;
	displacement = displacement * 2;
	//displacement = distex.r + distex.g + distex.b;
	input.position = input.position * (1 + displacement);
	
	//set w to one for proper matrix calculations.
    input.position.w = 1.0f;
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	//store uv coords and normal vector for pixel shader
    output.tex = input.tex;
	
	output.normal = input.normal;


    return output;
}