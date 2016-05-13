#include "VelocityRotator.h"



VelocityRotator::VelocityRotator(float scale)
	: Component()
{
	speedScale = scale;
	body = nullptr;
}


VelocityRotator::~VelocityRotator()
{
}

void VelocityRotator::ParentSet()
{
	body = GetEntity()->GetComponent<PhysicsBody>();
}

void VelocityRotator::Update(float dt)
{
	if (body != nullptr)
	{
		XMFLOAT3 rot = GetTransform().GetRotation();
		XMFLOAT4 velo = body->GetVelocity();
		rot.z -= velo.x * speedScale * dt;
		rot.x += velo.z * speedScale * dt;
		GetTransform().SetRotation(rot);
	}
}
