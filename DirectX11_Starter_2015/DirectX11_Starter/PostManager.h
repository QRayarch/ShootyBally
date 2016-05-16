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

	ID3D11RenderTargetView** GetChainRTVStart() { return &rtvOriginal; }
	ID3D11ShaderResourceView** GetChainSRVStart() { return &srvOriginal; }

	void SetChainDest(ID3D11RenderTargetView* rtv);

	void BuildResources(int windowWidth, int windowHeight);
	void ReleaseResources();
	void RunChain(int windowWidth, int windowHeight, ID3D11SamplerState* samplerState, UINT stride, UINT offset);
	void LoadShaders();

private:
	ID3D11RenderTargetView* rtvOriginal;
	ID3D11ShaderResourceView* srvOriginal;
	ID3D11RenderTargetView* rtvFinal;

	// blur
	ID3D11RenderTargetView* rtvGBlur1;
	ID3D11ShaderResourceView* srvGBlur1;
	ID3D11RenderTargetView* rtvGBlur2;
	ID3D11ShaderResourceView* srvGBlur2;

	// Bloom
	ID3D11RenderTargetView* rtvBloom;
	ID3D11ShaderResourceView* srvBloom;
	//ID3D11RenderTargetView* rtvBloomResult;
	//ID3D11ShaderResourceView* srvBloomResult;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	SimpleVertexShader* vertexShader;
	SimplePixelShader* psBlur;
	SimplePixelShader* psBloomThreshold;
	SimplePixelShader* psBloom;

	void BuildResourcePair(int windowWidth, int windowHeight, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv);
	void unbindResources();

	int blurAmount;
};

