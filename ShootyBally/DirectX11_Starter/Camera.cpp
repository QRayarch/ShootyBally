#include "Camera.h"
#include "Logger.h"

Camera::Camera()
{
	RecalculateViewMatrix();
}

Camera::Camera(float xPos, float yPos, float zPos)
{
	transform.SetPosition(DirectX::XMFLOAT3(xPos, yPos, zPos));
	RecalculateViewMatrix();
}


Camera::~Camera()
{
}

void Camera::Update(float dt, LONG x, LONG y)
{

	//Temp Camera Control in here
	float moveSpeed = 3.4f;
	float rotateSpeed = 9.9f;
	float xAxisMovement = 0;
	float zAxisMovement = 0;
	DirectX::XMFLOAT3 newPos = transform.GetPosition();
	DirectX::XMFLOAT3 newRot = transform.GetRotation();
	if (GetAsyncKeyState('W') & 0x8000) { zAxisMovement += moveSpeed * dt; }
	if (GetAsyncKeyState('S') & 0x8000) { zAxisMovement -= moveSpeed * dt; }
	if (GetAsyncKeyState('A') & 0x8000) { xAxisMovement -= moveSpeed * dt; }
	if (GetAsyncKeyState('D') & 0x8000) { xAxisMovement += moveSpeed * dt; }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { newPos.y += moveSpeed * dt; }
	if (GetAsyncKeyState('X') & 0x8000) { newPos.y -= moveSpeed * dt; }
	transform.SetPosition(newPos);
	transform.MoveRelative(xAxisMovement, 0.0f, zAxisMovement);
	newRot.x += y * rotateSpeed * dt;
	newRot.y += x * rotateSpeed * dt;
	if (newRot.x <= -1) {
		newRot.x = -1;
	}
	else if (newRot.x >= 1) {
		newRot.x = 1;
	}
	transform.SetRotation(newRot);

	//We changed position or rotaion, we need to update the view matrix
	/*if (transform.GetIsDirty()) {
		RecalculateViewMatrix();
	}*/
}

//Note: does not check to see if the transform actually updated
DirectX::XMFLOAT4X4 Camera::RecalculateViewMatrix()
{
	if (!transform.GetIsDirty()) return viewMatrix;
	DirectX::XMMATRIX lookToMatrix = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&transform.GetPosition()),
		DirectX::XMLoadFloat3(&transform.GetForwardVector()),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixTranspose(lookToMatrix));
	return viewMatrix;
}

void Camera::CreatePerspectiveProjectionMatrix(float aspectRatio, float nearClippingPlane, float farClippingPlane)
{
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		nearClippingPlane,				  	// Near clip plane distance
		farClippingPlane);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}
