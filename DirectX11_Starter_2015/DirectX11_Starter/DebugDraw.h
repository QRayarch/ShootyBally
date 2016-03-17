#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleShader.h"
#include "Camera.h"

struct DebugVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

class DebugDraw
{
public:
	DebugDraw();
	~DebugDraw();
	static void SetUp(ID3D11Device* newDevice, ID3D11DeviceContext* newDeviceContext, Camera* newCamera);
	static void Release();

	static void DrawLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color);
	static void DrawRay(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color);

private:
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static Camera* camera;
	static SimpleVertexShader* vertexShader;
	static SimplePixelShader* pixelShader;
};
