cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{ 
	float3 position		: POSITION;
	float4 color		: COLOR0;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR0;
};

VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;
	matrix worldViewProj = mul(view, projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.color = input.color;
	return output;
}