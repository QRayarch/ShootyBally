// Constant Buffer
// - Allows us to define a buffer of individual variables 
//    which will (eventually) hold data from our C++ code
// - All non-pipeline variables that get their values from 
//    our C++ code must be defined inside a Constant Buffer
// - The name of the cbuffer itself is unimportant
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // object space
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;		// uv coordinate
	float3 tangent		: TANGENT;
};

// Struct representing the data we're sending down the pipeline
// - Should match our geometry shader's input (hence the name: Vertex to Geometry)
// - At a minimum, we need a piece of data defined tagged as SV_POSITION
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexToGeometry
{
	// Data type
	//  |
	//  |   Name				 Semantic
	//  |    |				    	|
	//  v    v					    v
	float3 worldPos			: POSITION;		// world space
	float3 worldNorm		: NORMAL;
	float3 worldTang		: TANGENT;

	float2 uv				: TEXCOORD;		// uv coordinate
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToGeometry main(VertexShaderInput input)
{
	// Set up output struct
	VertexToGeometry output;

	//convert object space to world space once here as object space is unnecesary
	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;
	output.worldNorm = normalize(mul(input.normal, (float3x3)world));
	output.worldTang = normalize(mul(input.tangent, (float3x3)world));

	output.uv = input.uv;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}