#pragma once
#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <DirectXMath.h>
#include "Component.h"
#include "Transform.h"

using namespace DirectX;

class PhysicsBody : public Component
{
public:
	PhysicsBody();
	PhysicsBody(Transform* transform, float mass, const XMFLOAT4& gravitationalAcc = XMFLOAT4(0.0f, -9.81f, 0.0f, 0.0f));
	~PhysicsBody();

	float GetMass() const { return mass; }
	XMFLOAT4 GetVelocity() const { return velocity; }
	void SetVelocity(const XMFLOAT4& vel) { velocity = vel; }
	XMFLOAT4 GetAcceleration() const { return acceleration; }

	void Update() override;

	// Updates the associated transform's position and rotation.
	void PhysicsUpdate(float deltaTime);

	// Adds a force to the PhysicsBody which will be applied during update.
	void XM_CALLCONV AddForce(FXMVECTOR force);
	void AddForce(const XMFLOAT4& force);

private:
	XMFLOAT4 gravitationalAcc;
	XMFLOAT4 velocity;
	XMFLOAT4 acceleration;
	XMFLOAT4 netForce;
	Transform* transform;
	float mass;
};

inline void XM_CALLCONV PhysicsBody::AddForce(FXMVECTOR force)
{
	XMStoreFloat4(&netForce, XMVectorAdd(XMLoadFloat4(&netForce), force));
}

#endif
