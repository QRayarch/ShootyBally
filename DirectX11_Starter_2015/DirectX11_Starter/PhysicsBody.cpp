#include "PhysicsBody.h"

PhysicsBody::PhysicsBody()
	: PhysicsBody(nullptr, 1.0f)
{
}

PhysicsBody::PhysicsBody(Transform* transform, float mass, const XMFLOAT4& gravitationalAcc)
	: Component(),
	gravitationalAcc(gravitationalAcc),
	velocity(0.0f, 0.0f, 0.0f, 0.0f),
	acceleration(0.0f, 0.0f, 0.0f, 0.0f),
	netForce(0.0f, 0.0f, 0.0f, 0.0f),
	transform(transform),
	mass(mass)
{
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::Update()
{
	Component::Update();
}

void PhysicsBody::PhysicsUpdate(float deltaTime)
{
	AddForce(gravitationalAcc);

	XMVECTOR vel = XMLoadFloat4(&velocity);
	XMVECTOR netF = XMLoadFloat4(&netForce);

	XMVECTOR acc = XMVectorScale(netF, 1.0f / mass);
	vel = XMVectorAdd(vel, XMVectorScale(acc, deltaTime));
	transform->Translate(XMVectorScale(vel, deltaTime));

	XMStoreFloat4(&velocity, vel);
	XMStoreFloat4(&acceleration, acc);
	XMStoreFloat4(&netForce, XMVectorZero());
}

void PhysicsBody::AddForce(const XMFLOAT4& force)
{
	XMStoreFloat4(&netForce, XMVectorAdd(XMLoadFloat4(&netForce), XMLoadFloat4(&force)));
}