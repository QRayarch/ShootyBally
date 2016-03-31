#include "DebugDraw.h"
#include <PrimitiveBatch.h>

ID3D11Device* DebugDraw::device = nullptr;
ID3D11DeviceContext* DebugDraw::deviceContext = nullptr;
Camera* DebugDraw::camera = nullptr;
SimpleVertexShader* DebugDraw::vertexShader = nullptr;
SimplePixelShader* DebugDraw::pixelShader = nullptr;

DebugDraw::DebugDraw()
{
	
}


DebugDraw::~DebugDraw()
{
}

void DebugDraw::SetUp(ID3D11Device * newDevice, ID3D11DeviceContext * newDeviceContext, Camera* newCamera)
{
	device = newDevice;
	deviceContext = newDeviceContext;
	camera = newCamera;

	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VS_Debug.cso");
	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PS_Debug.cso");
}

void DebugDraw::Release()
{
	if (vertexShader != nullptr) {
		delete vertexShader;
		vertexShader = nullptr;
	}
	if (pixelShader != nullptr) {
		delete pixelShader;
		pixelShader = nullptr;
	}
}

void DebugDraw::DrawLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color)
{
	//This code will really get cleaned up

	//TODO: switch to a dynamic buffer, that doesn't get recreated every frame
	DebugVertex vertices[] = { {startPos, color}, {endPos, color} };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(DebugVertex) * 2;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	ID3D11Buffer* vertexBuffer;
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	UINT stride = sizeof(DebugVertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	vertexShader->SetMatrix4x4(0, DirectX::XMFLOAT4X4(	1, 0, 0, 0, 
														0, 1, 0, 0, 
														0, 0, 1, 0, 
														0, 0, 0, 1));
	vertexShader->SetMatrix4x4(1, camera->GetViewMatrix());
	vertexShader->SetMatrix4x4(2, camera->GetProjectionMatrix());
	vertexShader->SetShader(true);
	pixelShader->SetShader(true);
	deviceContext->Draw(2, 0);


	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	vertexBuffer->Release();
}