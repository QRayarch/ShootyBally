//default shading files
#include "DefaultVS.hlsl"
#include "DefaultPS.hlsl"
#include 

//toon shading files
#include "ToonVS.hlsl"
#include "ToonGS.hlsl"
#include "ToonPS.hlsl"
fxgroup shadingStyles
{
	technique11 defaultStyle
	{
		pass TheOnlyOne
		{
			SetVertexShader(CompileShader(vs_5_0, DefaultVS()));
			SetGeometryShader(NULL)
			SetPixilShader(CompileShader(ps_5_0), DefaultPS()));
		};
	};

	technique11 toonStyle
	{
		pass TheOnlyOne
		{
			SetVertexShader(CompileShader(vs_5_0, ToonVS()));
			SetGeometryShader(CompileShader(v_5_0, ToonGS()));
			SetPixilShader(CompileShader(ps_5_0), ToonPS()));
		};
	};
};