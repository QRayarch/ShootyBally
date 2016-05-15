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
}

void PostManager::SetChainDest(ID3D11RenderTargetView * rtv)
{
	rtvFinal = rtv;
	//srvFinal = srv;
}

void PostManager::BuildResources(float windowWidth, float windowHeight)
{
	BuildResourcePair(windowWidth, windowHeight, &rtvOriginal, &srvOriginal);
	BuildResourcePair(windowWidth, windowHeight, &rtvGBlur1, &srvGBlur1);
	BuildResourcePair(windowWidth, windowHeight, &rtvGBlur2, &srvGBlur2);
}

void PostManager::ReleaseResources()
{
	ReleaseMacro(rtvOriginal);
	ReleaseMacro(srvGBlur1);
	ReleaseMacro(srvGBlur2);
	ReleaseMacro(rtvGBlur1);
	ReleaseMacro(rtvGBlur2);
}

void PostManager::RunChain(float windowWidth, float windowHeight, ID3D11SamplerState* samplerState, UINT stride, UINT offset)
{
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	
	// Set targets for blur
	deviceContext->OMSetRenderTargets(1, &rtvGBlur1, 0);
	deviceContext->ClearRenderTargetView(rtvGBlur1, color);

	// Draw the post process
	vertexShader->SetShader();

	psBlur->SetBool("vertical", true);
	psBlur->SetInt("blurAmount", blurAmount);
	psBlur->SetFloat("pixelWidth", 1.0f / windowWidth);
	psBlur->SetFloat("pixelHeight", 1.0f / windowHeight);
	psBlur->SetShaderResourceView("pixels", srvOriginal);
	psBlur->SetSamplerState("trilinear", samplerState);
	psBlur->SetShader();

	// Turn off existing vert/index buffers
	ID3D11Buffer* nothing = 0;
	deviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(3, 0);

	// Unbind the SRV so the underlying texture isn't bound for
	// both input and output at the start of next frame
	psBlur->SetShaderResourceView("pixels", 0);

	//blur horizontal
	deviceContext->OMSetRenderTargets(1, &rtvFinal, 0);
	deviceContext->ClearRenderTargetView(rtvFinal, color);

	vertexShader->SetShader();

	psBlur->SetBool("vertical", false);
	psBlur->SetInt("blurAmount", blurAmount);
	psBlur->SetFloat("pixelWidth", 1.0f / windowWidth);
	psBlur->SetFloat("pixelHeight", 1.0f / windowHeight);
	psBlur->SetShaderResourceView("pixels", srvGBlur1);
	psBlur->SetSamplerState("trilinear", samplerState);
	psBlur->SetShader();

	deviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->Draw(3, 0);
}

void PostManager::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VS_Post.cso");
	psBlur = new SimplePixelShader(device, deviceContext);
	psBlur->LoadShaderFile(L"PS_Blur.cso");
}

void PostManager::BuildResourcePair(float windowWidth, float windowHeight, ID3D11RenderTargetView ** rtv, ID3D11ShaderResourceView ** srv)
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
