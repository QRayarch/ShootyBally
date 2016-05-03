
Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	//float4(input.uv, 0, 1); //
	return  diffuseTexture.Sample(samplerState, input.uv);
}