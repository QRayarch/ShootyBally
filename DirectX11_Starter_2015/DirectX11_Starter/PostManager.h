#pragma once

#include "Windows.h"
#include <d3d11.h>
#include <vector>
#include "SimpleShader.h"

#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

class PostManager
{
public:
	PostManager(ID3D11Device* deviceIn, ID3D11DeviceContext* deviceContextIn);
	~PostManager();

	ID3D11RenderTargetView* GetChainRTVStart() { return rtvOriginal; }
	ID3D11ShaderResourceView* GetChainSRVStart() { return srvOriginal; }

	void SetChainDest(ID3D11RenderTargetView* rtv);

	void BuildResources(float windowWidth, float windowHeight);
	void ReleaseResources();
	void RunChain(float windowWidth, float windowHeight, ID3D11SamplerState* samplerState, UINT stride, UINT offset);
	void LoadShaders();

private:
	ID3D11RenderTargetView* rtvOriginal;
	ID3D11ShaderResourceView* srvOriginal;
	ID3D11RenderTargetView* rtvFinal;

	ID3D11RenderTargetView* rtvGBlur1;
	ID3D11ShaderResourceView* srvGBlur1;
	ID3D11RenderTargetView* rtvGBlur2;
	ID3D11ShaderResourceView* srvGBlur2;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	SimpleVertexShader* vertexShader;
	SimplePixelShader* psBlur;

	void BuildResourcePair(float windowWidth, float windowHeight, ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* srv);
};

