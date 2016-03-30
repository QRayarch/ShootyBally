#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Transform
{
public:
	Transform();
	Transform(const Transform& other);
	Transform& operator=(const Transform& other);
	~Transform();

	//Getter and setters
	void SetPosition(XMFLOAT3 newPos);
	void MoveRelative(float addX, float addY, float addZ);
	void SetRotation(XMFLOAT3 newRot);
	void SetScale(XMFLOAT3 newScale);
	void SetParent(Transform* newParent);
	bool GetIsDirty();
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetForwardVector();
	XMFLOAT4X4 GetWorldMatrix() { return RecalculateWorldMatrix(); isDirty = false; }
	Transform* GetParent() { return parent; }

	XMFLOAT4X4 RecalculateWorldMatrix();

	void XM_CALLCONV Translate(FXMVECTOR translation);
	void Translate(const XMFLOAT3& translation);
	void Translate(float x, float y, float z);

	void Rotate(float x, float y, float z);

	void XM_CALLCONV Scale(FXMVECTOR scale);
	void Scale(const XMFLOAT3& scale);
	void Scale(float x, float y, float z);

private:
	//we save the world matrix and only recalculate it when this is true, this is set to true when we change information about the transform
	Transform* parent;
	bool isDirty;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	XMFLOAT4X4 worldMatrix;
};

inline void XM_CALLCONV Transform::Translate(FXMVECTOR translation)
{
	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), translation));

	isDirty = true;
}

inline void XM_CALLCONV Transform::Scale(FXMVECTOR scale)
{
	XMStoreFloat3(&this->scale, XMVectorMultiply(XMLoadFloat3(&this->scale), scale));

	isDirty = true;
}
