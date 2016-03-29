cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw          : TEXCOORD0;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix viewWithoutTranslation = view;
	viewWithoutTranslation._41 = 0;
	viewWithoutTranslation._42 = 0;
	viewWithoutTranslation._43 = 0;

	output.position = mul(float4(input.position, 1.0f), mul(viewWithoutTranslation, projection));

	output.position.z = output.position.w;

	output.uvw = input.position;

	return output;
}