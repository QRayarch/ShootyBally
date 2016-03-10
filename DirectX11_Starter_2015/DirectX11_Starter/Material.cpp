#include "Material.h"
#include "Render.h"
#include "Logger.h"

Material::Material(SimpleVertexShader* newVertexShader, 
					SimplePixelShader* newPixelShader, 
					ID3D11ShaderResourceView* newDiffuseSRV,
					ID3D11ShaderResourceView* newNormalMapSRV,
					ID3D11SamplerState* newSamplerState)
{
	vertexShader = newVertexShader;
	pixelShader = newPixelShader;
	diffuseTextureSRV = newDiffuseSRV;
	normalMapSRV = newNormalMapSRV;
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
		pixelShader->SetShaderResourceView(0, diffuseTextureSRV);
		pixelShader->SetShaderResourceView(1, normalMapSRV);
		pixelShader->SetSamplerState(0, samplerState);
		pixelShader->SetShader(true);

		renderInfo.currentMaterial = this;
	}
	else {
		vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
	}
}
