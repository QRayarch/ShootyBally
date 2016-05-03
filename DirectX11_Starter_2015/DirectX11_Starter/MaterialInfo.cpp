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

void MaterialInfo::AddFloat3(ShaderInfoElement<DirectX::XMFLOAT3> newData)
{
	float3s.push_back(newData);
}

void MaterialInfo::SetShaderData(ISimpleShader * shader)
{
	for (int f = 0; f < floats.size(); ++f) {
		shader->SetFloat(floats[f].shaderIndex, floats[f].data);
	}
	for (int f3 = 0; f3 < float3s.size(); ++f3) {
		shader->SetFloat3(float3s[f3].shaderIndex, float3s[f3].data);
	}
}
