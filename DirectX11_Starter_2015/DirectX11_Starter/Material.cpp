#include "Material.h"
#include "Render.h"
#include "Logger.h"

Material::Material(SimpleVertexShader* newVertexShader,
	SimpleGeometryShader* newGeometryShader,
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
		geometryShader = newGeometryShader;
		pixelShader = newPixelShader;
		for (unsigned int t = 0; t < numberOfTextures; ++t) {
			if (newTexture[t] != nullptr) {
				textures[t] = newTexture[t];
			}
		}
		samplerState = newSamplerState;
	}
	//infoForVertex = MaterialInfo();
	//infoForPixel = MaterialInfo();
}

Material::Material(SimpleVertexShader* newVertexShader,
	SimpleGeometryShader* newGeometryShader,
	SimplePixelShader* newPixelShader,
	ID3D11ShaderResourceView* newTexture,
	ID3D11SamplerState* newSamplerState)
{
	numberOfTextures = 0;
	vertexShader = newVertexShader;
	geometryShader = newGeometryShader;
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
		infoForVertex.SetShaderData(vertexShader);

		vertexShader->SetShader(true);

		if (geometryShader != NULL)
		{
			geometryShader->SetMatrix4x4(0, renderInfo.viewMatrix);
			geometryShader->SetMatrix4x4(1, renderInfo.projectionMatrix);
			geometryShader->SetData(2, &toonPerpCheck, sizeof(float));
			geometryShader->SetData(3, &toonOutlineThickness, sizeof(float));
			geometryShader->SetShader(true);
		}
		else
		{
			renderInfo.deviceContext->GSSetShader(0, 0, 0);
		}

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

		renderInfo.currentMaterial = this;
	}
	else {
		vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
	}
}

void Material::PrepareMaterial(RenderInfo & renderInfo, Transform & transform, MaterialInfo & indivVertexInfo, MaterialInfo & indivPixelInfo)
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
	bool needsRefresh = false;
	if (renderInfo.currentMaterial != this) {
		vertexShader->SetMatrix4x4(1, renderInfo.viewMatrix);
		vertexShader->SetMatrix4x4(2, renderInfo.projectionMatrix);
		infoForVertex.SetShaderData(vertexShader);
		vertexShader->SetShader(true);

		pixelShader->SetFloat3(0, renderInfo.cameraPosition);
		pixelShader->SetData(1, &renderInfo.light1, sizeof(RenderLight));
		pixelShader->SetData(2, &renderInfo.light2, sizeof(RenderLight));
		for (unsigned int t = 0; t < numberOfTextures; ++t) {
			pixelShader->SetShaderResourceView(t, textures[t]);
		}
		infoForPixel.SetShaderData(pixelShader);

		pixelShader->SetSamplerState(0, samplerState);
		pixelShader->SetShader(true);

		renderInfo.currentMaterial = this;
	}
	else
	{
		needsRefresh = true;
	}
	if (indivVertexInfo.SetShaderData(vertexShader) || indivPixelInfo.SetShaderData(pixelShader)) {
		needsRefresh = true;
	}
	if (needsRefresh) {
		vertexShader->CopyBufferData(0);//The world matrix needs to be set per object
		pixelShader->CopyBufferData(0);
	}
}
