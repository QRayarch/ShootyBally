#include "MaterialInfo.h"
#include "SimpleShader.h"


MaterialInfo::MaterialInfo()
{
	/*floats = new ShaderInfoElement<float>[MAX_NUM_SHADER_INFO_FLOAT];
	for (int f = 0; f < MAX_NUM_SHADER_INFO_FLOAT; ++f) {
		floats[f].data = 0;
		floats[f].shaderIndex = 0;
	}
	float3s = new ShaderInfoElement<DirectX::XMFLOAT3>[MAX_NUM_SHADER_INFO_FLOAT3];
	for (int f = 0; f < MAX_NUM_SHADER_INFO_FLOAT3; ++f) {
		float3s[f].data = DirectX::XMFLOAT3(0, 0, 0);
		float3s[f].shaderIndex = 0;
	}*/
	array = new float[20];
	numFloat = 0;
	numFloat3 = 0;
}


MaterialInfo::~MaterialInfo()
{
	if (array != nullptr) {
		delete[] array;
		array = nullptr;
	}
	/*if (floats != nullptr) {
		delete[] floats;
		floats = nullptr;
	}
	if (float3s != nullptr) {
		delete[] float3s;
		float3s = nullptr;
	}*/
}

void MaterialInfo::AddFloat(ShaderInfoElement<float> newData)
{
	/*if (numFloat < MAX_NUM_SHADER_INFO_FLOAT) {
		floats[numFloat] = newData;
		numFloat += 1;
	}*/
}

ShaderInfoElement<float>* MaterialInfo::GetFloatByShaderIndex(int i)
{
	/*for (int f = 0; f < numFloat; ++f) {
		if (floats[f].shaderIndex == i) {
			return &floats[f];
		}
	}*/
	return nullptr;
}

void MaterialInfo::AddFloat3(ShaderInfoElement<DirectX::XMFLOAT3> newData)
{
	/*if (numFloat3 < MAX_NUM_SHADER_INFO_FLOAT3) {
		float3s[numFloat3] = newData;
		numFloat3 += 1;
	}*/
}

ShaderInfoElement<DirectX::XMFLOAT3>* MaterialInfo::GetFloat3ByShaderIndex(int i)
{
	/*for (int f = 0; f < numFloat3; ++f) {
		if (float3s[f].shaderIndex == i) {
			return &float3s[f];
		}
	}*/
	return nullptr;
}

void MaterialInfo::SetShaderData(ISimpleShader * shader)
{
	/*for (int f = 0; f < numFloat; ++f) {
		shader->SetFloat(floats[f].shaderIndex, floats[f].data);
	}
	for (int f3 = 0; f3 < numFloat3; ++f3) {
		shader->SetFloat3(float3s[f3].shaderIndex, float3s[f3].data);
	}*/
}
