#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(EntitySystem* entsys, int entIndex)
{
	bulletEntity = entsys->GetEntity(entIndex);
	lifespan = 5.0f;
	velocity = 5.0f;
}

Bullet::~Bullet()
{
}

bool Bullet::GetIsActive()
{
	return isActive;
}

Entity* Bullet::GetEntity()
{
	return bulletEntity;
}

void Bullet::UpdatePhysics(float deltaTime)
{
	bulletEntity->GetComponent<PhysicsBody>()->PhysicsUpdate(deltaTime);
	lifespanLeft -= deltaTime;
	if (lifespanLeft <= 0) {
		isActive = false;
	}
}

void Bullet::Fire(Transform playerTransform)
{
	isActive = true;
	lifespanLeft = lifespan;

	//Transform bulletTransform = bulletEntity->GetTransform();
	bulletEntity->GetTransform().SetPosition(playerTransform.GetPosition());
	bulletEntity->GetTransform().SetRotation(playerTransform.GetRotation());
	
	//TODO: Make the bullet fire in the direction the player is facing
	XMVECTOR newVel = XMLoadFloat3(&(bulletEntity->GetTransform().GetForwardVector()));
	XMVector3Normalize(newVel);
	XMFLOAT3 velocity;
	XMStoreFloat3(&velocity, XMVector3Normalize(newVel) * 3);
	bulletEntity->GetComponent<PhysicsBody>()->SetVelocity(XMFLOAT4(velocity.x, velocity.y, velocity.z, 0.0f));
}