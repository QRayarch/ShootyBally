#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "Transform.h"
//Temp
#include <Windows.h>

class Camera
{
public:
	Camera();
	Camera(float xPos, float yPos, float zPos);
	~Camera();

	void Update(float dt, LONG x, LONG y);
	DirectX::XMFLOAT4X4 RecalculateViewMatrix();

	void CreatePerspectiveProjectionMatrix(float aspectRatio, float nearClippingPlane, float farClippingPlane);

	DirectX::XMFLOAT4X4 GetViewMatrix() { return RecalculateViewMatrix(); }
	DirectX::XMFLOAT4X4 GetProjectionMatrix() { return projectionMatrix; }
	Transform& GetTransform() { return transform; }
private:
	Transform transform;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

