struct VertexToPixel
{
	float4 position			: SV_POSITION;
	float3 uvw				: TEXCOORD0;
};

TextureCube cubeMap			: register(t0);
SamplerState samplerState	: register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	return cubeMap.Sample(samplerState, input.uvw);
}