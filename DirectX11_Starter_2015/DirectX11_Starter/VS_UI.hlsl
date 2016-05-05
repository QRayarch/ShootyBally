cbuffer externalData : register(b0)
{
	matrix world;//0
	matrix view;//1
	matrix projection;//2
	float3 scale;//3
	float3 color;//4
	float aspectRatio;//5
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
	float3 color		: COLOR0;
	float2 uv			: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	input.position.z = 0;
	float ar = (scale.x / scale.y) * aspectRatio;
	input.position.x -= input.normal.x * ((1 - abs(input.position.x)) / (ar) * sign(input.position.x));
	input.position.y -= input.normal.x * ((1 - abs(input.position.y)) / (1/ar)* sign(input.position.y));

	output.position = mul(float4(input.position, 1.0f), world);
	output.position.z = 0;
	output.uv = input.uv;
	output.color = color;

	return output;
}