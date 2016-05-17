#define TOON
struct Light {
	float4 AmbientColor;
	float4 DiffuseColor;
	//Holds differnet data depending on type
	//Holds direction for Direction Light
	//Holds position for point Light
	float3 Fluid3;
	float Intensity;
	//0 is Directional Light
	//1 is Point Light
	int Type;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalMap : register(t1);
SamplerState samplerState : register(s0);

cbuffer extData : register(b0)
{
	matrix view;
	Light light1;
	Light light2;

	//extra Toon Shader info
	float4 lineColor;
	float numBreaks;
	float perpCheck;
};

struct GeometryToPixel
{
	// Data type
	//  |
	//  |   Name				 Semantic
	//  |    |				    	|
	//  v    v					    v
	float4 screenPos		: SV_POSITION;	// Screen Coordinates

	float3 worldPos			: POSITION;		// world space
	float3 worldNorm		: NORMAL;
	float3 worldTang		: TANGENT;

	float2 uv				: TEXCOORD;		// uv coordinate

	bool outline			: OUTLINE;		// determines if outline vertex
};

float4 CalculateLight(Light light, GeometryToPixel input, inout float4 baseColor) : COLOR0
{
	float nDotL = 0;
baseColor += light.AmbientColor;
if (light.Type == 0)// Direction
{
	nDotL = dot(input.worldNorm, normalize(-light.Fluid3));

}
else if (light.Type == 1)// Point
{
	//float3 dirToPointLight = normalize(light.Fluid3 - input.worldPos);
	//float pointNdotL = saturate(dot(input.normal, dirToPointLight));
	//nDotL = pointNdotL;
	nDotL = dot(input.worldNorm, normalize(light.Fluid3 - input.worldPos)) / length(light.Fluid3 - input.worldPos);
}
nDotL *= light.Intensity;
#ifdef TOON
nDotL = smoothstep(0, 0.03f, nDotL * 0.08);
#endif

return (light.DiffuseColor * saturate(nDotL) * baseColor);
}

float4 CalculateRimLighting(float3 dirToCamera, float3 theViewNorm) : COLOR0
{
	//FREN
	/*float bias = 0.56;
	float scale = 0.17f;
	float power = 3;
	float r = 1 - saturate(bias + scale * pow(1 + dot(dirToCamera, input.normal), power));*/
	float base = 0.75f - dot(dirToCamera, theViewNorm);
	float exp = pow(base, 5);
	float r = exp + 0.0f * (1 - exp);
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

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(GeometryToPixel input) : SV_TARGET
{
	//obtain view space norm and tang
	//normalize world norm and tangent to be safe
	input.worldNorm = normalize(input.worldNorm);
	input.worldTang = normalize(input.worldTang);

	//calculate view space position and norm
	float3 viewPos = mul(float4(input.worldPos, 1.0f), view).xyz;
	float3 viewNorm = normalize(mul(input.worldNorm, (float3x3)view));

	//direction to Camera in viewSpace
	float3 dirToCamera = normalize(-viewPos);
	
	//if outline face check on a pixel level for perpendicularity
	float howPerp = saturate(dot(dirToCamera, viewNorm));
	if (input.outline && howPerp < perpCheck)
	{
		return lineColor;
	}
	else
	{
		float4 baseColor = diffuseTexture.Sample(samplerState, input.uv);
		baseColor = baseColor + CalculateRimLighting(dirToCamera, viewNorm);
		//return CalculateRimLighting(dirToCamera, input);
	
		float3 refl = reflect(-dirToCamera, viewNorm);
	
		float4 lights = float4(0.0f, 0.0f, 0.0f, 1.0f);
		lights += CalculateLight(light1, input, baseColor);
		lights += CalculateLight(light2, input, baseColor);

		//toon shading relies on sharper jumps in color
		//the following uses numBreaks to determine the number of possible intensities
		//if numBreaks = 4 then each part of lights is set to 0, 0.25, 0.5, 0.75, 1
		//if numBreaks = 10 then each part of lights is set to 0.1, 0.2, 0.3, etc. to 1
		float4 finalShade = asfloat(trunc(lights * numBreaks) / numBreaks);
		return finalShade;
	}
}