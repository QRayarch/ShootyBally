#include "Transform.h"


Transform::Transform()
{
	isDirty = true;
	parent = nullptr;
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}

Transform::Transform(const Transform & other)
{
	isDirty = other.isDirty;
	parent = other.parent;
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
	worldMatrix = other.worldMatrix;
}

Transform & Transform::operator=(const Transform & other)
{
	if (this == &other) return *this;
	isDirty = other.isDirty;
	parent = other.parent;
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
	worldMatrix = other.worldMatrix;
	return *this;
}


Transform::~Transform()
{
}

void Transform::SetPosition(XMFLOAT3 newPos)
{
	isDirty = true;
	position = newPos;
}

void Transform::MoveRelative(float addX, float addY, float addZ)
{
	isDirty = true;
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(addX, addY, addZ, 0.0f),
		XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)));
	XMStoreFloat3(&position, XMVectorAdd(dir, XMLoadFloat3(&position)));
}

void Transform::SetRotation(XMFLOAT3 newRot)
{
	isDirty = true;
	rotation = newRot;
}

void Transform::SetScale(XMFLOAT3 newScale)
{
	isDirty = true;
	scale = newScale;
}

void Transform::SetParent(Transform * newParent)
{
	if (newParent == nullptr || newParent == this) return;
	//TODO: test this function, circular dependency issues should be fixed, but only test on trying to set a transforms parrent to itself
	Transform* current = newParent;
	while(current->GetParent() != nullptr) {
		if (current == this) return;
		current = current->GetParent();
	}
	parent = newParent;
}

bool Transform::GetIsDirty()
{
	if (parent == nullptr) {
		return isDirty;
	}
	return isDirty || parent->GetIsDirty();
}

XMFLOAT3 Transform::GetForwardVector()
{
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)));
	XMFLOAT3 realForward;
	XMStoreFloat3(&realForward, forward);
	return realForward;
}

XMFLOAT4X4 Transform::RecalculateWorldMatrix()
{
	//TODO: figure out a better way to handle rotaions
	if (!GetIsDirty()) return worldMatrix;//Dont know if I want to keep it.
	XMMATRIX allignedWorldMatrix = XMLoadFloat4x4(&worldMatrix);
	if (parent == nullptr) {
		XMMATRIX  calculatedWorldMatrix =
			XMMatrixScalingFromVector(XMLoadFloat3(&scale)) * //XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)) *
			//XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z) *
			XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation))) * 
			XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(calculatedWorldMatrix));
	} else {
		XMMATRIX  calculatedWorldMatrix =
			XMMatrixScalingFromVector(XMLoadFloat3(&scale)) * //XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)) *
			XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation))) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(
			XMMatrixMultiply(XMMatrixTranspose(XMLoadFloat4x4(&parent->GetWorldMatrix())), calculatedWorldMatrix)));
	}
	return worldMatrix;
}

void Transform::Translate(const XMFLOAT3& translation)
{
	XMStoreFloat3(&position, XMVectorAdd(XMLoadFloat3(&position), XMLoadFloat3(&translation)));

	isDirty = true;
}

void Transform::Translate(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;

	isDirty = true;
}

void Transform::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;

	isDirty = true;
}

void Transform::Scale(const XMFLOAT3& scale)
{
	XMStoreFloat3(&this->scale, XMVectorMultiply(XMLoadFloat3(&this->scale), XMLoadFloat3(&scale)));

	isDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	this->scale.x *= x;
	this->scale.y *= y;
	this->scale.z *= z;

	isDirty = true;
}
