#pragma once
#include "SimpleShader.h"
#include "Transform.h"
#include "MaterialInfo.h"
enum ShadingTechnique { Default, Toon, UI };
struct RenderInfo;



class Material
{
public:
	const static unsigned int MAX_NUM_TEXTURES = 3;

	//This constructor should probably be changed
	Material(SimpleVertexShader* newVertexShader,
			SimplePixelShader* newPixelShader,
			SimpleGeometryShader* newGeometryShader,
			int theTechnique,
			ID3D11ShaderResourceView** newTexture,
			unsigned int newNumText,
			ID3D11SamplerState* newSamplerState);
	Material(SimpleVertexShader* newVertexShader, 
			SimplePixelShader* newPixelShader,
			int theTechnique,
			ID3D11ShaderResourceView** newTexture,
			unsigned int newNumText,
			ID3D11SamplerState* newSamplerState);
	Material(SimpleVertexShader* newVertexShader,
		SimplePixelShader* newPixelShader,
		ID3D11ShaderResourceView* newTexture,
		ID3D11SamplerState* newSamplerState);
	~Material();

	SimpleVertexShader* GetVertexShader() { return vertexShader; }
	void SetVertexShader(SimpleVertexShader* newVertexShader) { vertexShader = newVertexShader;  }
	SimplePixelShader* GetPixelShader() { return pixelShader; }
	void SetPixelShader(SimplePixelShader* newPixelShader) { pixelShader = newPixelShader; }
	SimpleGeometryShader* GetGeometryShader() { return geometryShader; }
	void SetGeometryShader(SimpleGeometryShader* newGeometryShader) { geometryShader = newGeometryShader; }
	void SetSamplerState(ID3D11SamplerState* newSamplerState) { samplerState = newSamplerState; }
	void PrepareMaterial(RenderInfo& renderInfo, Transform& transform);
	//void PrepareMaterial(RenderInfo& renderInfo, Transform& transform, MaterialInfo& indivVertexInfo, MaterialInfo& indivPixelInfo);

	MaterialInfo* GetVertexMaterialInfo() { return &infoForVertex; }
	MaterialInfo* GetPixelMaterialInfo() { return &infoForPixel; }
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleGeometryShader* geometryShader;
	int technique;
	//Stuff for handeling shader info
	MaterialInfo infoForVertex;
	MaterialInfo infoForPixel;

	//Stuff for textures
	ID3D11ShaderResourceView* textures[MAX_NUM_TEXTURES];
	unsigned int numberOfTextures;
	ID3D11SamplerState* samplerState;//Sampler

	//Toon Shading
	float toonPerpCheck = 0.05f;
	float toonOutlineThickness = 0.1f;
	XMFLOAT4 toonLineColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	float toonNumBreaks = 5.0f;
};

