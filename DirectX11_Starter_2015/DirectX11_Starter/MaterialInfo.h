#pragma once
#include "SimpleShader.h"
#include <vector>

template<class T>
struct ShaderInfoElement {
	int shaderIndex;
	T data;
};

class MaterialInfo
{
public:
	const static unsigned int MAX_NUM_SHADER_INFO_FLOAT = 3;
	const static unsigned int MAX_NUM_SHADER_INFO_FLOAT3 = 1;

	MaterialInfo();
	~MaterialInfo();

	void AddFloat(ShaderInfoElement<float> newData);
	ShaderInfoElement<float>& GetFloat(int i) { return floats[i]; }
	int GetFloatLength() { return numFloat; }
	ShaderInfoElement<float>* GetFloatByShaderIndex(int i);
	void AddFloat3(ShaderInfoElement<DirectX::XMFLOAT3> newData);
	ShaderInfoElement<DirectX::XMFLOAT3>& GetFloat3(int i) { return float3s[i]; }
	int GetFloat3Length() { return numFloat3; }
	ShaderInfoElement<DirectX::XMFLOAT3>* GetFloat3ByShaderIndex(int i);

	void SetShaderData(ISimpleShader* shader);
private:
	ShaderInfoElement<float>* floats;
	float* array;
	unsigned int numFloat;
	ShaderInfoElement<DirectX::XMFLOAT3>* float3s;
	unsigned int numFloat3;
	//std::vector<ShaderInfoElement<float>> floats;
	//std::vector<ShaderInfoElement<DirectX::XMFLOAT3>> float3s;
};

