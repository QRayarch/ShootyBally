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
	const static int MAX_NUMBER_OF_DEBUG_VERTS = 5000;

	DebugDraw();
	~DebugDraw();
	static void SetUp(ID3D11Device* newDevice, ID3D11DeviceContext* newDeviceContext, Camera* newCamera);
	static void Release();

	static void AddLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color);
	static void AddBox(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT4 color);
	static void AddSphere(DirectX::XMFLOAT3 position, float radius, DirectX::XMFLOAT4 color);

	static void DrawAll(bool changesTyplogyBack);
private:
	static ID3D11Device* device;
	static ID3D11DeviceContext* deviceContext;
	static Camera* camera;
	static SimpleVertexShader* vertexShader;
	static SimplePixelShader* pixelShader;
	static int numVerts;
	static DebugVertex vertices[MAX_NUMBER_OF_DEBUG_VERTS];
};
