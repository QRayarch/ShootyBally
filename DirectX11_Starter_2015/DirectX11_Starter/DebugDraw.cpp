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

void DebugDraw::DrawRay(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, DirectX::XMFLOAT4 color)
{
	DrawLine(startPos, endPos, color);

	/*float arrowHeight = 0.1f;
	float arrowRadius = 0.05f;

	//This code will really get cleaned up
	DirectX::XMVECTOR vEndPos = DirectX::XMLoadFloat3(&endPos);
	DirectX::XMVECTOR vDirVec = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(vEndPos, DirectX::XMLoadFloat3(&startPos))), arrowHeight);
	DirectX::XMVECTOR vBase = DirectX::XMVectorAdd(vEndPos, vDirVec);

	DirectX::XMFLOAT3 vert1;
	DirectX::XMFLOAT3 vert2;
	DirectX::XMFLOAT3 vert3;
	DirectX::XMFLOAT3 vert4;

	DirectX::XMStoreFloat3(&vert1, DirectX::XMVectorAdd(vBase, DirectX::XMVectorScale(DirectX::XMVectorSet(0.7f, 0, 0.7f, 0) ,arrowRadius)));
	DirectX::XMStoreFloat3(&vert2, DirectX::XMVectorAdd(vBase, DirectX::XMVectorScale(DirectX::XMVectorSet(0.7f, 0, -0.7f, 0), arrowRadius)));
	DirectX::XMStoreFloat3(&vert3, DirectX::XMVectorAdd(vBase, DirectX::XMVectorScale(DirectX::XMVectorSet(-0.7f, 0, -0.7f, 0), arrowRadius)));
	DirectX::XMStoreFloat3(&vert4, DirectX::XMVectorAdd(vBase, DirectX::XMVectorScale(DirectX::XMVectorSet(-0.7f, 0, 0.7f, 0), arrowRadius)));

	DebugVertex vertices[] = {	{ endPos, color },{ vert1, color },{ vert2, color },//0		1	2
								{ endPos, color },{ vert2, color },{ vert3, color },//3		4	5
								{ endPos, color },{ vert3, color },{ vert4, color },//6		7	8
								{ endPos, color },{ vert4, color },{ vert1, color } //9		10	11
	};

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


	UINT indices[] = {	0, 1, 2,
						3, 4, 5,
						6, 7, 8,
						9, 10, 11 };

	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	vertexShader->SetMatrix4x4(0, DirectX::XMFLOAT4X4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1));
	vertexShader->SetMatrix4x4(1, camera->GetViewMatrix());
	vertexShader->SetMatrix4x4(2, camera->GetProjectionMatrix());
	vertexShader->SetShader(true);
	pixelShader->SetShader(true);
	deviceContext->Draw(2, 0);

	vertexBuffer->Release();
	indexBuffer->Release();*/
}

