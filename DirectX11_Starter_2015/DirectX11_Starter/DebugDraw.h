#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "PrimitiveBatch.h"
#include "VertexTypes.h"

class DebugDraw
{
public:
	DebugDraw();
	~DebugDraw();
	static void SetUp(ID3D11Device* newDevice, ID3D11DeviceContext* newDeviceContext);
	static void Release();

	static void DrawLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT3 color);

private:
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
};
