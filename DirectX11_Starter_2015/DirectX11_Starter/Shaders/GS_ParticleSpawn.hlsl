#define TYPE_ROOT 0
#define TYPE_PARTICLE 1

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

struct GSOutput
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

cbuffer externalData : register(b0)
{
	float dt;
	float ageToSpawn;
	float maxLifetime;
	float totalTime;
	int numSpawn;
}

texture1D randomTexture	: register(t0);
sampler randomSampler	: register(s0);

[maxvertexcount(32)]
void main(point VStoGS input[1], inout PointStream<GSOutput> outStream)
{
	// Increment the age.
	input[0].age += dt;

	// Root particle?
	if (input[0].type == TYPE_ROOT)
	{
		if (numSpawn != 0)
		{
			for (int i = 0; i < numSpawn; ++i)
			{
				// Make a copy to emit.
				GSOutput emit;
				emit.type = TYPE_PARTICLE;
				emit.age = 0;
				emit.startPos = input[0].startPos.xyz;
				emit.startVel = input[0].startVel;
				emit.startColor = input[0].startColor;
				emit.midColor = input[0].midColor;
				emit.endColor = input[0].endColor;
				emit.sizes = input[0].sizes;

				// Alter some values from default.
				float4 random = randomTexture.SampleLevel(randomSampler, totalTime * 10 * i, 0);
				emit.startPos += random.xyz * 0.5f;
				emit.startVel.x = random.w * 9.0f;
				emit.startVel.y += random.w * 5.0f;
				emit.startVel.z = random.x * 9.0f;

				outStream.Append(emit);
			}
		}

		// Time for new particle?
		if (input[0].age >= ageToSpawn)
		{
			// Reset root age.
			input[0].age = 0;

			// Make a copy to emit.
			GSOutput emit;
			emit.type		= TYPE_PARTICLE;
			emit.age		= 0;
			emit.startPos	= input[0].startPos.xyz;
			emit.startVel	= input[0].startVel;
			emit.startColor	= input[0].startColor;
			emit.midColor	= input[0].midColor;
			emit.endColor	= input[0].endColor;
			emit.sizes		= input[0].sizes;

			// Alter some values from default.
			float4 random = randomTexture.SampleLevel(randomSampler, totalTime * 10, 0);
			emit.startPos += random.xyz * 0.5f;
			emit.startVel.x = random.w * 0.3f;
			emit.startVel.z = random.x * 0.3f;

			outStream.Append(emit);
		}

		// Always keep root particle.
		GSOutput root;
		root.type = input[0].type;
		root.age = input[0].age;
		root.startPos = input[0].startPos.xyz;
		root.startVel = input[0].startVel;
		root.startColor = input[0].startColor;
		root.midColor = input[0].midColor;
		root.endColor = input[0].endColor;
		root.sizes = input[0].sizes;

		outStream.Append(root);
	}
	else if (input[0].age < maxLifetime)
	{
		// Just a regular particle - keep if age is ok.
		GSOutput particle;
		particle.type = input[0].type;
		particle.age = input[0].age;
		particle.startPos = input[0].startPos.xyz;
		particle.startVel = input[0].startVel;
		particle.startColor = input[0].startColor;
		particle.midColor = input[0].midColor;
		particle.endColor = input[0].endColor;
		particle.sizes = input[0].sizes;

		outStream.Append(particle);
	}
}
