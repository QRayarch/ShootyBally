#pragma once
#include "SimpleShader.h"
#include "Transform.h"

struct RenderInfo;

class Material
{
public:
	const static unsigned int MAX_NUM_TEXTURES = 3;

	//This constructor should probably be changed
	Material(SimpleVertexShader* newVertexShader, 
			SimplePixelShader* newPixelShader, 
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
	void SetSamplerState(ID3D11SamplerState* newSamplerState) { samplerState = newSamplerState; }
	void PrepareMaterial(RenderInfo& renderInfo, Transform& transform);
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	//Stuff for textures
	ID3D11ShaderResourceView* textures[MAX_NUM_TEXTURES];
	unsigned int numberOfTextures;
	ID3D11SamplerState* samplerState;//Sampler
};

