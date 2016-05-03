struct VSInput
{
	int type			: TEXCOORD0;
	float age			: TEXCOORD1;
	float3 startPos		: POSITION;
	float3 startVel		: TEXCOORD2;
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float3 sizes		: TEXCOORD3;
};

struct VStoGS
{
	int type			: TEXCOORD0;
	float age			: TEXCOORD1;
	float4 startPos		: SV_POSITION;
	float3 startVel		: TEXCOORD2;
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float3 sizes		: TEXCOORD3;
};

// The entry point for the vertex shader.
VStoGS main(VSInput input)
{
	VStoGS output;
	output.type = input.type;
	output.age = input.age;
	output.startPos = float4(input.startPos, 1.0f);
	output.startVel = input.startVel;
	output.startColor = input.startColor;
	output.midColor = input.midColor;
	output.endColor = input.endColor;
	output.sizes = input.sizes;

	return output;
}
