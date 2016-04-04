#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(EntitySystem* entsys, int entIndex)
{
	bulletEntity = entsys->GetEntity(entIndex);
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
}

void Bullet::Fire(Transform playerTransform)
{
	isActive = true;

	//Transform bulletTransform = bulletEntity->GetTransform();
	bulletEntity->GetTransform().SetPosition(playerTransform.GetPosition());
	bulletEntity->GetTransform().SetRotation(playerTransform.GetRotation());
	
	//TODO: Make the bullet fire in the direction the player is facing
	bulletEntity->GetComponent<PhysicsBody>()->SetVelocity(XMFLOAT4(5.0f, 0.0f, 0.0f, 0.0f));
}