#pragma once
#include "SimpleShader.h"

#include "Transform.h"

struct RenderInfo;

class Material
{
public:
	//This constructor should probably be changed
	Material(SimpleVertexShader* newVertexShader, 
			SimplePixelShader* newPixelShader, 
			ID3D11ShaderResourceView* newDiffuseSRV,
			ID3D11ShaderResourceView* newNormalMapSRV,
			ID3D11SamplerState* newSamplerState);
	~Material();

	void SetVertexShader(SimpleVertexShader* newVertexShader) { vertexShader = newVertexShader;  }
	void SetPixelShader(SimplePixelShader* newPixelShader) { pixelShader = newPixelShader; }
	void SetDiffuseSRV(ID3D11ShaderResourceView* newDiffuseSRV) { diffuseTextureSRV = newDiffuseSRV;  }
	void SetNormalMapSRV(ID3D11ShaderResourceView* newNormalMapSRV) { normalMapSRV = newNormalMapSRV; }
	void SetSamplerState(ID3D11SamplerState* newSamplerState) { samplerState = newSamplerState; }
	void PrepareMaterial(RenderInfo& renderInfo, Transform& transform);
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	//Stuff for textures
	ID3D11ShaderResourceView* diffuseTextureSRV;//Texture
	ID3D11ShaderResourceView* normalMapSRV;//Texture
	ID3D11SamplerState* samplerState;//Sampler
};

