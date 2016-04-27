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

void PhysicsBody::ResolveCollisions(PhysicsBody* otherPB)
{
	Entity* ourEnt = GetEntity();
	Entity* otherEnt = otherPB->GetEntity();

	XMFLOAT3 distanceVec;
	XMStoreFloat3(&distanceVec, XMLoadFloat3(&ourEnt->GetTransform().GetPosition()) - XMLoadFloat3(&otherEnt->GetTransform().GetPosition()));
	XMStoreFloat3(&distanceVec, XMVector3Normalize(XMLoadFloat3(&distanceVec)));

	//OtherEnt needs to change to an entity, will do later
	float newVelX1 = (velocity.x * (mass - otherPB->GetMass()) + (2 * otherPB->GetMass() * otherPB->GetVelocity().x)) / (mass + otherPB->GetMass());
	float newVelZ1 = (velocity.z * (mass - otherPB->GetMass()) + (2 * otherPB->GetMass() * otherPB->GetVelocity().z)) / (mass + otherPB->GetMass());

	float newVelX2 = (otherPB->GetVelocity().x * (otherPB->GetMass() - mass) + (2 * mass * velocity.x)) / (mass + otherPB->GetMass());
	float newVelZ2 = (otherPB->GetVelocity().z * (otherPB->GetMass() - mass) + (2 * mass * velocity.z)) / (mass + otherPB->GetMass());

	SetVelocity(XMFLOAT4(newVelX1, 0.0f, newVelZ1, 0.0f));
	//XMFLOAT3 currentPosition = GetEntity()->GetTransform().GetPosition();
	//GetEntity()->GetTransform().SetPosition(XMFLOAT3(currentPosition.x + newVelX1, currentPosition.y + 0.0f, currentPosition.z + newVelZ1));
	
	otherPB->SetVelocity(XMFLOAT4(newVelX2 * distanceVec.x, 0.0f, newVelZ2 * distanceVec.z, 0.0f));
	//XMFLOAT3 otherPosition = otherEnt->GetEntity()->GetTransform().GetPosition();
	//otherEnt->GetEntity()->GetTransform().SetPosition(XMFLOAT3(otherPosition.x + newVelX2, otherPosition.y + 0.0f, otherPosition.z + newVelZ2));
}

void PhysicsBody::AddForce(const XMFLOAT4& force)
{
	XMStoreFloat4(&netForce, XMVectorAdd(XMLoadFloat4(&netForce), XMLoadFloat4(&force)));
}
