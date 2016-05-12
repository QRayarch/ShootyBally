#include "Material.h"
#include "Render.h"
#include "Logger.h"

Material::Material(SimpleVertexShader* newVertexShader,
	SimplePixelShader* newPixelShader,
	SimpleGeometryShader* newGeometryShader,
	int theTechnique,
	ID3D11ShaderResourceView** newTexture,
	unsigned int newNumText,
	ID3D11SamplerState* newSamplerState)
{
	if (newNumText > MAX_NUM_TEXTURES) {
		LogText("--ERROR creating material--//Trying to create a texture with more than the max number of allowed textures.");
		numberOfTextures = 0;
		vertexShader = nullptr;
		pixelShader = nullptr;
		geometryShader = NULL;
		samplerState = nullptr;
	}
	else {
		numberOfTextures = newNumText;
		vertexShader = newVertexShader;
		pixelShader = newPixelShader;
		geometryShader = newGeometryShader;
		technique = theTechnique;
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
	int theTechnique,
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
		geometryShader = NULL;
		technique = theTechnique;
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
	SimpleGeometryShader* newGeometryShader,
	int theTechnique,
	ID3D11ShaderResourceView* newTexture,
	ID3D11SamplerState* newSamplerState)
{
	numberOfTextures = 0;
	vertexShader = newVertexShader;
	pixelShader = newPixelShader;
	geometryShader = newGeometryShader;
	technique = theTechnique;
	if (newTexture != nullptr) {
		textures[0] = newTexture;
		numberOfTextures = 1;
	}
	samplerState = newSamplerState;
}

Material::Material(SimpleVertexShader* newVertexShader,
	SimplePixelShader* newPixelShader,
	int theTechnique,
	ID3D11ShaderResourceView* newTexture,
	ID3D11SamplerState* newSamplerState)
{
	numberOfTextures = 0;
	vertexShader = newVertexShader;
	pixelShader = newPixelShader;
	geometryShader = NULL;
	technique = theTechnique;
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
	switch (technique)
	{
	case (ShadingTechnique::Default) :
	{
		//The numbers only work if everything is passed in correctly into the shader 

		//"world" = 0
		//"view" = 1
		//"proj" = 2

		//"cameraPosition" = 0
		//"light1" = 1
		//"light2" = 2

		//diffuseTexture 0
		//normalMap 1
		//samplerState 0
		vertexShader->SetMatrix4x4(0, transform.GetWorldMatrix());
		if (renderInfo.currentMaterial != this) {
			renderInfo.deviceContext->GSSetShader(0, 0, 0);

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

			renderInfo.deviceContext->RSSetState(0);
			renderInfo.deviceContext->OMSetDepthStencilState(0, 0);

			renderInfo.deviceContext->Flush();

			renderInfo.currentMaterial = this;
		}
		else {
			vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
		}
		break;
	}
	case (ShadingTechnique::Toon) :
	{
		//The numbers only work if everything is passed in correctly into the shader 

		//"world" = 0

		//"view" = 0
		//"proj" = 1
		//"perpCheck" = 2
		//"outlineThickness" = 3

		//"view" = 0
		//"light1" = 1
		//"light2" = 2
		//"lineColor" = 3
		//"numBreaks" = 4
		//"perpCheck" = 5

		//diffuseTexture 0
		//normalMap 1
		//samplerState 0
		vertexShader->SetMatrix4x4(0, transform.GetWorldMatrix());
		geometryShader->SetMatrix4x4(0, renderInfo.viewMatrix);
		pixelShader->SetMatrix4x4(0, renderInfo.viewMatrix);
		if (renderInfo.currentMaterial != this) {
			vertexShader->SetShader(true);

			geometryShader->SetMatrix4x4(0, renderInfo.viewMatrix);
			geometryShader->SetMatrix4x4(1, renderInfo.projectionMatrix);
			geometryShader->SetData(2, &toonPerpCheck, sizeof(float));
			geometryShader->SetData(3, &toonOutlineThickness, sizeof(float));
			geometryShader->SetShader(true);

			pixelShader->SetMatrix4x4(0, renderInfo.viewMatrix);
			pixelShader->SetData(1, &renderInfo.light1, sizeof(RenderLight));
			pixelShader->SetData(2, &renderInfo.light2, sizeof(RenderLight));
			pixelShader->SetData(3, &toonLineColor, sizeof(XMFLOAT4));
			pixelShader->SetData(4, &toonNumBreaks, sizeof(float));
			pixelShader->SetData(5, &toonPerpCheck, sizeof(float));
			for (unsigned int t = 0; t < numberOfTextures; ++t) {
				pixelShader->SetShaderResourceView(t, textures[t]);
			}

			//pixelShader->SetShaderResourceView(1, normalMapSRV);
			pixelShader->SetSamplerState(0, samplerState);
			pixelShader->SetShader(true);

			renderInfo.deviceContext->RSSetState(0);
			renderInfo.deviceContext->OMSetDepthStencilState(0, 0);

			renderInfo.deviceContext->Flush();

			renderInfo.currentMaterial = this;
		}
		else {
			vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
		}
		break;
	}
	case (ShadingTechnique::UI) :
	{
		if (renderInfo.currentMaterial != this) {
			renderInfo.deviceContext->GSSetShader(0, 0, 0);
		}
		break;
	}
	}
}

/*void Material::PrepareMaterial(RenderInfo & renderInfo, Transform & transform, MaterialInfo & indivVertexInfo, MaterialInfo & indivPixelInfo)
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
		infoForVertex.SetShaderData(vertexShader);
		indivVertexInfo.SetShaderData(vertexShader);
		vertexShader->SetShader(true);

		pixelShader->SetFloat3(0, renderInfo.cameraPosition);
		pixelShader->SetData(1, &renderInfo.light1, sizeof(RenderLight));
		pixelShader->SetData(2, &renderInfo.light2, sizeof(RenderLight));
		for (unsigned int t = 0; t < numberOfTextures; ++t) {
			pixelShader->SetShaderResourceView(t, textures[t]);
		}
		infoForPixel.SetShaderData(pixelShader);
		indivPixelInfo.SetShaderData(pixelShader);

		pixelShader->SetSamplerState(0, samplerState);
		pixelShader->SetShader(true);

		renderInfo.currentMaterial = this;
	}
	else {
		vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
	}
}*/
