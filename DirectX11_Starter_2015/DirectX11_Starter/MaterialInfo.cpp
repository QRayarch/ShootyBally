#include "MaterialInfo.h"



MaterialInfo::MaterialInfo()
{
}


MaterialInfo::~MaterialInfo()
{
}

void MaterialInfo::AddFloat(ShaderInfoElement<float> newData)
{
	floats.push_back(newData);
}

ShaderInfoElement<float>* MaterialInfo::GetFloatByShaderIndex(int i)
{
	for (int f = 0; f < floats.size(); ++f) {
		if (floats[f].shaderIndex == i) {
			return &floats[f];
		}
	}
	return nullptr;
}

void MaterialInfo::AddFloat3(ShaderInfoElement<DirectX::XMFLOAT3> newData)
{
	float3s.push_back(newData);
}

ShaderInfoElement<DirectX::XMFLOAT3>* MaterialInfo::GetFloat3ByShaderIndex(int i)
{
	for (int f = 0; f < float3s.size(); ++f) {
		if (float3s[f].shaderIndex == i) {
			return &float3s[f];
		}
	}
	return nullptr;
}

bool MaterialInfo::SetShaderData(ISimpleShader * shader)
{
	for (int f = 0; f < floats.size(); ++f) {
		shader->SetFloat(floats[f].shaderIndex, floats[f].data);
	}
	for (int f3 = 0; f3 < float3s.size(); ++f3) {
		shader->SetFloat3(float3s[f3].shaderIndex, float3s[f3].data);
	}
	return floats.size() != 0 || float3s.size() != 0;
}
