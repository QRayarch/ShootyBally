//#define TOON

struct Light {
	float4 AmbientColor;
	float4 DiffuseColor;
	//Holds differnet data depending on type
	//Holds direction for Direction Light
	//Holds position for point Light
	float3 Fluid3;
	//0 is Directional Light
	//1 is Point Light
	int Type;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalMap : register(t1);
SamplerState samplerState : register(s0);

cbuffer extData : register(b0)
{
	float3 cameraPosition;
	Light light1;
	Light light2;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
};

float4 CalculateLight(Light light, VertexToPixel input, inout float4 baseColor) : COLOR0
{
	float nDotL = 0;
	baseColor += light.AmbientColor;
	if (light.Type == 0)// Direction
	{
		nDotL = dot(input.normal, normalize(-light.Fluid3));

	} 
	else if (light.Type == 1)// Point
	{
		//float3 dirToPointLight = normalize(light.Fluid3 - input.worldPos);
		//float pointNdotL = saturate(dot(input.normal, dirToPointLight));
		//nDotL = pointNdotL;
		nDotL = dot(input.normal, normalize(light.Fluid3 - input.worldPos)) / length(light.Fluid3 - input.worldPos);
	}

#ifdef TOON
	nDotL = smoothstep(0, 0.03f, nDotL);
#endif

	return (light.DiffuseColor * saturate(nDotL) * baseColor);
}

float4 CalculateRimLighting(float3 dirToCamera, VertexToPixel input) : COLOR0
{
	//FREN
	/*float bias = 0.56;
	float scale = 0.17f;
	float power = 3;
	float r = 1 - saturate(bias + scale * pow(1 + dot(dirToCamera, input.normal), power));*/
	float base = 1 - dot(dirToCamera, input.normal);
	float exp = pow(base, 5);
	float r = exp + 0 * (1 - exp);
	return r.xxxx;
}

float4 CalculateSpecular(float3 dirToCamera, float3 reflection) : COLOR0
{
	float3 spec = pow(max(dot(reflection, dirToCamera), 0), 64);// *0.5f;
#ifdef TOON
	spec = smoothstep(0, 0.03f, spec);
#endif
	return spec.xxxx;
}

float3 CalculateNormalFromMap(VertexToPixel input) : NORMAL
{
	//Get the normal from the map and unpack it to the range [-1, 1]
	float3 mapNormal = normalMap.Sample(samplerState, input.uv).rgb * 2 - 1;
	//Make sure the normal and the tangent are orthogonal 
	float3 tangent = normalize(input.tangent - input.normal * dot(input.tangent, input.normal));
	return float3(normalize(mul(mapNormal,
		//Tangent, BiTangent and Normal Matrix
		float3x3(tangent, cross(tangent, input.normal), input.normal)
		)));
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	//Normalize inputs
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	//return float4(CalculateNormalFromMap(input), 1);
	input.normal = CalculateNormalFromMap(input);

	float3 dirToCamera = normalize(cameraPosition - input.worldPos);

	float4 baseColor = diffuseTexture.Sample(samplerState, input.uv);
	baseColor = baseColor + CalculateRimLighting(dirToCamera, input);
	//return CalculateRimLighting(dirToCamera, input);

	float3 refl = reflect(-dirToCamera, input.normal);

	float4 lights = float4(0.0f, 0.0f, 0.0f, 1.0f);
	lights += CalculateLight(light1, input, baseColor);
	lights += CalculateLight(light2, input, baseColor);


	return  lights + CalculateSpecular(dirToCamera, refl);
}