// Defines the input to this pixel shader.
struct GStoPS
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD0;
};

// Textures.
Texture2D particleTexture	: register(t0);
SamplerState trilinear		: register(s0);

// Entry point for the pixel shader.
float4 main(GStoPS input) : SV_TARGET
{
	return particleTexture.Sample(trilinear, input.uv) * input.color;
}
