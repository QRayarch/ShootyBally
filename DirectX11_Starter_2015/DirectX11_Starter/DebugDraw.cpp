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

void DebugDraw::DrawLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT3 color)
{

}
