#include "PostManager.h"

PostManager::PostManager(ID3D11Device * deviceIn, ID3D11DeviceContext * deviceContextIn)
{
	device = deviceIn;
	deviceContext = deviceContextIn;

	blurAmount = 5;

	rtvOriginal = nullptr;
	srvGBlur1 = nullptr;
	srvGBlur2 = nullptr;
	rtvGBlur1 = nullptr;
	rtvGBlur2 = nullptr;
}

PostManager::~PostManager()
{
	ReleaseResources();
	delete psBlur;
	delete vertexShader;
	delete psBloom;
	delete psBloomThreshold;
}

void PostManager::SetChainDest(ID3D11RenderTargetView * rtv)
{
	rtvFinal = rtv;
	//srvFinal = srv;
}

void PostManager::BuildResources(int windowWidth, int windowHeight)
{
	BuildResourcePair(windowWidth, windowHeight, &rtvOriginal, &srvOriginal);
	BuildResourcePair(windowWidth, windowHeight, &rtvGBlur1, &srvGBlur1);
	BuildResourcePair(windowWidth, windowHeight, &rtvGBlur2, &srvGBlur2);
	BuildResourcePair(windowWidth, windowHeight, &rtvBloom, &srvBloom);
	//BuildResourcePair(windowWidth, windowHeight, &rtvBloomResult, &srvBloomResult);
}

void PostManager::ReleaseResources()
{
	ReleaseMacro(rtvOriginal);
	ReleaseMacro(srvGBlur1);
	ReleaseMacro(srvGBlur2);
	ReleaseMacro(rtvGBlur1);
	ReleaseMacro(rtvGBlur2);
	ReleaseMacro(rtvBloom);
	//ReleaseMacro(rtvBloomResult);
	ReleaseMacro(srvBloom);
	//ReleaseMacro(srvBloomResult);
}

void PostManager::RunChain(int windowWidth, int windowHeight, ID3D11SamplerState* samplerState, UINT stride, UINT offset)
{
	//resetting stuff to function propperly
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	ID3D11Buffer* nothing = 0;
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Turn off existing vert/index buffers
	deviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	//Bloom
	deviceContext->OMSetRenderTargets(1, &rtvBloom, 0);
	deviceContext->ClearRenderTargetView(rtvBloom, color);

	vertexShader->SetShader();

	psBloomThreshold->SetFloat("threshold", 0.3f);
	psBloomThreshold->SetShaderResourceView("pixels", srvOriginal);
	psBloomThreshold->SetSamplerState("trilinear", samplerState);
	psBloomThreshold->SetShader();

	deviceContext->Draw(3, 0);
	psBloomThreshold->SetShaderResourceView("pixels", 0);

	
	// Set targets for blur
	deviceContext->OMSetRenderTargets(1, &rtvGBlur1, 0);
	deviceContext->ClearRenderTargetView(rtvGBlur1, color);

	// Draw the post process
	vertexShader->SetShader();

	psBlur->SetInt("vertical", 1);
	psBlur->SetInt("blurAmount", blurAmount);
	psBlur->SetFloat("pixelWidth", 1.0f / windowWidth);
	psBlur->SetFloat("pixelHeight", 1.0f / windowHeight);
	psBlur->SetShaderResourceView("pixels", srvBloom);
	psBlur->SetSamplerState("trilinear", samplerState);
	psBlur->SetShader();


	// Draw
	deviceContext->Draw(3, 0);

	// Unbind the SRV so the underlying texture isn't bound for
	// both input and output at the start of next frame
	psBlur->SetShaderResourceView("pixels", 0);

	//blur horizontal
	deviceContext->OMSetRenderTargets(1, &rtvFinal, 0);
	deviceContext->ClearRenderTargetView(rtvFinal, color);

	vertexShader->SetShader();

	psBlur->SetInt("vertical", 0);
	psBlur->SetInt("blurAmount", blurAmount);
	psBlur->SetFloat("pixelWidth", 1.0f / windowWidth);
	psBlur->SetFloat("pixelHeight", 1.0f / windowHeight);
	psBlur->SetShaderResourceView("pixels", srvGBlur1);
	psBlur->SetSamplerState("trilinear", samplerState);
	psBlur->SetShader();

	deviceContext->Draw(3, 0);

	//Final Bloom composite
	deviceContext->OMSetRenderTargets(1, &rtvFinal, 0);
	deviceContext->ClearRenderTargetView(rtvFinal, color);

	vertexShader->SetShader();

	psBloom->SetFloat("bloomIntensity", 1.3f);
	psBloom->SetFloat("originalIntensity", 1.0f);
	psBloom->SetFloat("bloomSaturation", 1.0f);
	psBloom->SetFloat("originalSaturation", 1.0f);
	psBloom->SetShaderResourceView("bloomThresh", srvGBlur2);
	psBloom->SetShaderResourceView("cleanImage", srvOriginal);
	psBloom->SetSamplerState("trilinear", samplerState);
	psBloom->SetShader();

	deviceContext->Draw(3, 0);
	psBloomThreshold->SetShaderResourceView("pixels", 0);
}

void PostManager::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VS_Post.cso");
	psBlur = new SimplePixelShader(device, deviceContext);
	psBlur->LoadShaderFile(L"PS_Blur.cso");
	psBloom = new SimplePixelShader(device, deviceContext);
	psBloom->LoadShaderFile(L"PS_Bloom.cso");
	psBloomThreshold = new SimplePixelShader(device, deviceContext);
	psBloomThreshold->LoadShaderFile(L"PS_BloomThreshold.cso");
}

void PostManager::BuildResourcePair(int windowWidth, int windowHeight, ID3D11RenderTargetView ** rtv, ID3D11ShaderResourceView ** srv)
{
	//Target Texture
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = windowWidth;
	texDesc.Height = windowHeight;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* postTexture;
	device->CreateTexture2D(&texDesc, 0, &postTexture);

	//Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = texDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(postTexture, &rtvDesc, rtv);

	//Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(postTexture, &srvDesc, srv);

	//Release Texture cleanup
	ReleaseMacro(postTexture);
}
