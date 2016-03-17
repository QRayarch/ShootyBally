#include "DebugDraw.h"
#include <PrimitiveBatch.h>
#include "Vertex.h"

ID3D11Device* DebugDraw::device = nullptr;
ID3D11DeviceContext* DebugDraw::deviceContext = nullptr;

DebugDraw::DebugDraw()
{
	
}


DebugDraw::~DebugDraw()
{
}

void DebugDraw::SetUp(ID3D11Device * newDevice, ID3D11DeviceContext * newDeviceContext)
{
	device = newDevice;
	deviceContext = newDeviceContext;
}

void DebugDraw::Release()
{
	
}

void DebugDraw::DrawLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color)
{
	Vertex vertices[] = { {startPos}, {endPos} };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * 2;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	ID3D11Buffer* vertexBuffer;
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->Draw(2, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexBuffer->Release();
}

