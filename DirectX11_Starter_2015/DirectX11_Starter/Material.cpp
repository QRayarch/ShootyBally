#include "Material.h"
#include "Render.h"
#include "Logger.h"

Material::Material(SimpleVertexShader* newVertexShader, 
					SimplePixelShader* newPixelShader, 
					ID3D11ShaderResourceView** newTexture,
					unsigned int newNumText,
					ID3D11SamplerState* newSamplerState)
{
	if (newNumText > MAX_NUM_TEXTURES) {
		LogText("--ERROR creating material--//Trying to create a texture with more than the max number of allowed textures.");
		numberOfTextures = 0;
		vertexShader = nullptr;
		pixelShader = nullptr;
		samplerState = nullptr;
	}
	else {
		numberOfTextures = newNumText;
		vertexShader = newVertexShader;
		pixelShader = newPixelShader;
		for (unsigned int t = 0; t < numberOfTextures; ++t) {
			if (newTexture[t] != nullptr) {
				textures[t] = newTexture[t];
			}
		}
		samplerState = newSamplerState;
	}

}

Material::Material(SimpleVertexShader* newVertexShader,
	SimplePixelShader* newPixelShader,
	ID3D11ShaderResourceView* newTexture,
	ID3D11SamplerState* newSamplerState)
{
	numberOfTextures = 0;
	vertexShader = newVertexShader;
	pixelShader = newPixelShader;
	if (newTexture != nullptr) {
		textures[0] = newTexture;
		numberOfTextures = 1;
	}
	samplerState = newSamplerState;
}

Material::~Material()
{
}

void Material::PrepareMaterial(RenderInfo& renderInfo, Transform& transform)
{
	//The numbers only work if everything is passed in correctly into the shader 

	//"world" = 0
	//"view" = 1
	//"projection" = 2


	//"cameraPosition" = 0
	//"light1" = 1
	//"light2" = 2

	//diffuseTexture 0
	//normalMap 1
	//samplerState 0
	vertexShader->SetMatrix4x4(0, transform.GetWorldMatrix());
	if (renderInfo.currentMaterial != this) {
		vertexShader->SetMatrix4x4(1, renderInfo.viewMatrix);
		vertexShader->SetMatrix4x4(2, renderInfo.projectionMatrix);
		vertexShader->SetShader(true);

		pixelShader->SetFloat3(0, renderInfo.cameraPosition);
		pixelShader->SetData(1, &renderInfo.light1, sizeof(RenderLight));
		pixelShader->SetData(2, &renderInfo.light2, sizeof(RenderLight));
		for (unsigned int t = 0; t < numberOfTextures; ++t) {
			pixelShader->SetShaderResourceView(t, textures[t]);
		}

		//pixelShader->SetShaderResourceView(1, normalMapSRV);
		pixelShader->SetSamplerState(0, samplerState);
		pixelShader->SetShader(true);

		renderInfo.currentMaterial = this;
	}
	else {
		vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
	}
}
