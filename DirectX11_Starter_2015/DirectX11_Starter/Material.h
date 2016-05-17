#pragma once
#include "SimpleShader.h"
#include "Transform.h"
#include "MaterialInfo.h"

struct RenderInfo;



class Material
{
public:
	const static unsigned int MAX_NUM_TEXTURES = 3;

	//This constructor should probably be changed
	Material(SimpleVertexShader* newVertexShader,
		SimpleGeometryShader* newGeometryShader,
		SimplePixelShader* newPixelShader,
		ID3D11ShaderResourceView** newTexture,
		unsigned int newNumText,
		ID3D11SamplerState* newSamplerState);
	Material(SimpleVertexShader* newVertexShader,
		SimpleGeometryShader* newGeometryShader,
		SimplePixelShader* newPixelShader,
		ID3D11ShaderResourceView* newTexture,
		ID3D11SamplerState* newSamplerState);
	~Material();

	SimpleVertexShader* GetVertexShader() { return vertexShader; }
	void SetVertexShader(SimpleVertexShader* newVertexShader) { vertexShader = newVertexShader; }
	SimpleGeometryShader* GetGeometryShader() { return geometryShader; }
	void SetGeometryShader(SimpleGeometryShader* newGeometryShader) { geometryShader = newGeometryShader; }
	SimplePixelShader* GetPixelShader() { return pixelShader; }
	void SetPixelShader(SimplePixelShader* newPixelShader) { pixelShader = newPixelShader; }
	void SetSamplerState(ID3D11SamplerState* newSamplerState) { samplerState = newSamplerState; }
	void PrepareMaterial(RenderInfo& renderInfo, Transform& transform);
	void PrepareMaterial(RenderInfo& renderInfo, Transform& transform, MaterialInfo& indivVertexInfo, MaterialInfo& indivPixelInfo);

	MaterialInfo& GetVertexMaterialInfo() { return infoForVertex; }
	MaterialInfo& GetPixelMaterialInfo() { return infoForPixel; }
private:
	SimpleVertexShader* vertexShader;
	SimpleGeometryShader* geometryShader;
	SimplePixelShader* pixelShader;
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
	float toonNumBreaks = 3.0f;
};

