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
	//const static unsigned int MAX_NUM_SHADER_INFO_FLOAT = 1;
	//const static unsigned int MAX_NUM_SHADER_INFO_FLOAT3 = 1;

	MaterialInfo();
	~MaterialInfo();

	void AddFloat(ShaderInfoElement<float> newData);
	ShaderInfoElement<float>* GetFloat(int i) { return &floats[i]; }
	void AddFloat3(ShaderInfoElement<DirectX::XMFLOAT3> newData);

	void SetShaderData(ISimpleShader* shader);
private:
	/*ShaderInfoElement<float>* floatsData[MAX_NUM_SHADER_INFO_FLOAT];
	unsigned int numFloat;
	ShaderInfoElement<DirectX::XMFLOAT3>* float3sData[MAX_NUM_SHADER_INFO_FLOAT3];
	unsigned int numFloat3;*/
	std::vector<ShaderInfoElement<float>> floats;
	std::vector<ShaderInfoElement<DirectX::XMFLOAT3>> float3s;
};

