cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float aspectRatio;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	input.position.z = 0;
	//if (input.normal.x == 1) {
		input.position.x *= 1/aspectRatio;
		//input.position.y *= 1 / aspectRatio;
	//}

	output.position = mul(float4(input.position, 1.0f), world);
	output.position.z = 0;
	output.uv = input.uv;

	return output;
}