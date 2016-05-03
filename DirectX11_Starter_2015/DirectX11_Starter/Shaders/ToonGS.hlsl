
cbuffer externalData : register(b0)
{
	matrix view;
	matrix proj;
	float perpCheck = 0.2f;
	float outlineThickness = 1.0f;
};

////properties
//float perpCheck = 0.2f;
//float outlineThickness = 0.3f;


//input struct
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

//output struct
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

//takes all the triangles in a mesh and makes part of an inverted hollow shell to create an outline
[maxvertexcount(6)]
void main(triangle VertexToGeometry input[3], inout TriangleStream< GeometryToPixel > output)
{
	/*
	Go through each vertex in the incoming triangle.
	
	If the face is almost perpendicular to the camera:
		Copy the vertex, move it out along the normal.
		Invert the normal on this new vertex for the outline triangle.
	Otherwise:
		Only output the original three vertexes
	*/
	for (uint i = 0; i < 3; i++)
	{
		GeometryToPixel element;

		//put current vertex in elements and calcuate screen pos
		element.worldPos = input[i].worldPos;
		element.worldNorm = input[i].worldNorm;
		element.worldTang = input[i].worldTang;
		element.uv = input[i].uv;

		element.screenPos = mul(mul(float4(element.worldPos, 1.0f), view), proj);
		element.outline = false;

		output.Append(element);
	}

	output.RestartStrip();

	for (uint i = 0; i < 3; i++)
	{
		GeometryToPixel element;

		//obtain view space pos and norm
		float3 viewPos = mul(float4(input[i].worldPos, 1.0f), view).xyz;
		float3 viewNorm = normalize(mul(input[i].worldNorm, (float3x3)view));

		//obtain the normal to the camera
		//view space simplifies this
		float3 toCam = -normalize(viewPos);

		//check how perpendicular vertex is to camera
		float howPerp = saturate(dot(toCam, viewNorm));
		if (howPerp < perpCheck)
		{
			//create copy vertex if part of the border
			element.worldPos = input[i].worldPos;
			element.worldNorm = input[i].worldNorm;
			element.worldTang = input[i].worldTang;
			element.uv = input[i].uv;

			//move out along normal
			element.worldPos += outlineThickness*element.worldNorm;
			element.worldNorm *= -1.0f;

			//calculate screen pos
			element.screenPos = mul(mul(float4(element.worldPos, 1.0f), view), proj);
			
			element.outline = true;

			output.Append(element);
		}
	}
}