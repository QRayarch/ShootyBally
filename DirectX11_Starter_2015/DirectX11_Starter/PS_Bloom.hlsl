cbuffer Data : register(b0)
{
	float bloomIntensity;
	float originalIntensity;
	float bloomSaturation;
	float originalSaturation;
}

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

float4 AdjustSaturation(float4 color, float saturation)
{
	float gray = dot(color, float3(0.3, 0.3, 0.3));
	return lerp(gray, color, saturation);
}

// Textures to combine
Texture2D bloomThresh	: register(t0);
Texture2D cleanImage	: register(t1);
SamplerState trilinear	: register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 bloomColor = bloomThresh.Sample(trilinear, input.uv);
	float4 originalColor = cleanImage.Sample(trilinear, input.uv);

	bloomColor = AdjustSaturation(bloomColor, bloomSaturation) * bloomIntensity;
	originalColor = AdjustSaturation(originalColor, originalSaturation) * originalIntensity;

	originalColor *= (1 - saturate(bloomColor));

	return originalColor + bloomColor;
}