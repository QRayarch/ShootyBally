#include "DebugDraw.h"
#include <PrimitiveBatch.h>

ID3D11Device* DebugDraw::device = nullptr;
ID3D11DeviceContext* DebugDraw::deviceContext = nullptr;
Camera* DebugDraw::camera = nullptr;
SimpleVertexShader* DebugDraw::vertexShader = nullptr;
SimplePixelShader* DebugDraw::pixelShader = nullptr;
int DebugDraw::numVerts = 0;
DebugVertex DebugDraw::vertices[MAX_NUMBER_OF_DEBUG_VERTS];

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

void DebugDraw::AddLine(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color)
{
	if (numVerts + 2 < MAX_NUMBER_OF_DEBUG_VERTS) {
		vertices[numVerts].Position = startPos;
		vertices[numVerts].Color = color;

		vertices[numVerts + 1].Position = endPos;
		vertices[numVerts + 1].Color = color;
		numVerts += 2;
	}
}

void DebugDraw::AddBox(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, DirectX::XMFLOAT4 color)
{
	//Get the half size for simplified calculations
	DirectX::XMVECTOR halfSize = DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&size), DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 0.5f));
	DirectX::XMStoreFloat3(&size, halfSize);
	DirectX::HXMVECTOR vecPos = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR xAxis = DirectX::XMVectorSet(size.x, 0, 0, 0);
	DirectX::XMVECTOR yAxis = DirectX::XMVectorSet(0, size.y, 0, 0);
	DirectX::XMVECTOR zAxis = DirectX::XMVectorSet(0, 0, size.z, 0);
	//Temp variables 
	DirectX::XMFLOAT3 corner1;
	DirectX::XMFLOAT3 corner2;
	DirectX::XMFLOAT3 corner3;
	DirectX::XMFLOAT3 corner4;
	DirectX::XMFLOAT3 corner5;
	DirectX::XMFLOAT3 corner6;
	DirectX::XMFLOAT3 corner7;
	DirectX::XMFLOAT3 corner8;

	DirectX::XMStoreFloat3(&corner1, vecPos + xAxis + yAxis + zAxis);
	DirectX::XMStoreFloat3(&corner2, vecPos - xAxis + yAxis + zAxis);

	DirectX::XMStoreFloat3(&corner3, vecPos + xAxis - yAxis + zAxis);
	DirectX::XMStoreFloat3(&corner4, vecPos - xAxis - yAxis + zAxis);

	DirectX::XMStoreFloat3(&corner5, vecPos + xAxis + yAxis - zAxis);
	DirectX::XMStoreFloat3(&corner6, vecPos - xAxis + yAxis - zAxis);

	DirectX::XMStoreFloat3(&corner7, vecPos + xAxis - yAxis - zAxis);
	DirectX::XMStoreFloat3(&corner8, vecPos - xAxis - yAxis - zAxis);

	AddLine(corner1, corner2, color);
	AddLine(corner3, corner4, color);
	AddLine(corner5, corner6, color);
	AddLine(corner7, corner8, color);

	AddLine(corner1, corner3, color);
	AddLine(corner2, corner4, color);
	AddLine(corner5, corner7, color);
	AddLine(corner6, corner8, color);

	AddLine(corner1, corner5, color);
	AddLine(corner2, corner6, color);
	AddLine(corner3, corner7, color);
	AddLine(corner4, corner8, color);
}

void DebugDraw::AddSphere(DirectX::XMFLOAT3 position, float radius, DirectX::XMFLOAT4 color)
{
	float res = 24;
	float anglePerRes = DirectX::XM_2PI / res;//DirectX::XM_2PI / res;
	

	for (int x = 0; x < res; ++x) {
		float cosStart = 0;
		float sinStart = 0;
		DirectX::XMScalarSinCos(&sinStart, &cosStart, x * anglePerRes);
		float cosEnd = 0;
		float sinEnd = 0;
		DirectX::XMScalarSinCos(&sinEnd, &cosEnd, (x+1) * anglePerRes);
		cosStart *= radius;
		sinStart *= radius;
		cosEnd *= radius;
		sinEnd *= radius;

		//Didn't feel like dealing with the right way of doing the math
		//X Axis
		AddLine(DirectX::XMFLOAT3(position.x, cosStart + position.y, sinStart + position.z), DirectX::XMFLOAT3(position.x, cosEnd + position.y, sinEnd + position.z), color);
		//Y axis
		AddLine(DirectX::XMFLOAT3(cosStart + position.x, position.y, sinStart + position.z), DirectX::XMFLOAT3(cosEnd + position.x, position.y, sinEnd + position.z), color);
		//Z Axis
		AddLine(DirectX::XMFLOAT3(cosStart + position.x, sinStart + position.y, position.z), DirectX::XMFLOAT3(cosEnd + position.x, sinEnd + position.y, position.z), color);
	}
}

void DebugDraw::DrawAll(bool changesTyplogyBack)
{
	if (numVerts > 0) {
		//Ge the old typology so we can set it back.
		D3D11_PRIMITIVE_TOPOLOGY oldTypology;
		deviceContext->IAGetPrimitiveTopology(&oldTypology);

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.ByteWidth = sizeof(DebugVertex) * numVerts;
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
		vertexShader->SetMatrix4x4(0, DirectX::XMFLOAT4X4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1));
		vertexShader->SetMatrix4x4(1, camera->GetViewMatrix());
		vertexShader->SetMatrix4x4(2, camera->GetProjectionMatrix());
		vertexShader->SetShader(true);
		pixelShader->SetShader(true);
		deviceContext->Draw(numVerts, 0);
		if (changesTyplogyBack) {
			deviceContext->IASetPrimitiveTopology(oldTypology);
		}
		vertexBuffer->Release();
		numVerts = 0;
	}
}
