#pragma once
#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	Transform(const Transform& other);
	Transform& operator=(const Transform& other);
	~Transform();

	//Getter and setters
	void SetPosition(DirectX::XMFLOAT3 newPos);
	void MoveRelative(float addX, float addY, float addZ);
	void SetRotation(DirectX::XMFLOAT3 newRot);
	void SetScale(DirectX::XMFLOAT3 newScale);
	void SetParrent(Transform* newParrent);
	bool GetIsDirty();
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	DirectX::XMFLOAT3 GetForwardVector();
	DirectX::XMFLOAT4X4 GetWorldMatrix() { return RecalculateWorldMatrix(); isDirty = false; }
	Transform* GetParrent() { return parrent; }

	DirectX::XMFLOAT4X4 RecalculateWorldMatrix();
private:
	//we save the world matrix and only recalculate it when this is true, this is set to true when we change information about the transform
	Transform* parrent;
	bool isDirty;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4X4 worldMatrix;
};

