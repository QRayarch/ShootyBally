#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(Entity* entity, ParticleEmitter* trailParticleEmitter, ParticleEmitter* explosionParticleEmitter)
{
	bulletEntity = entity;
	this->trailParticleEmitter = trailParticleEmitter;
	this->explosionParticleEmitter = explosionParticleEmitter;
	lifespan = 4.0f;
}

Bullet::~Bullet()
{
}

void Bullet::SetIsActive(bool activity)
{
	isActive = activity;
	if (!activity)
	{
		trailParticleEmitter->Disable();
		explosionParticleEmitter->Enable();
		explosionParticleEmitter->Disable();
		GetEntity()->GetTransform().SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
	}
}

void Bullet::UpdatePhysics(float deltaTime)
{
	bulletEntity->GetComponent<PhysicsBody>()->PhysicsUpdate(deltaTime);
	lifespanLeft -= deltaTime;
	if (lifespanLeft <= 0) {
		isActive = false;
		trailParticleEmitter->Disable();
		explosionParticleEmitter->Enable();
		explosionParticleEmitter->Disable();
		GetEntity()->GetTransform().SetPosition(XMFLOAT3(0, 100, 0));
	}
}

void Bullet::Fire(Transform playerTransform)
{
	isActive = true;
	lifespanLeft = lifespan;

	bulletEntity->GetTransform().SetPosition(playerTransform.GetPosition());
	bulletEntity->GetTransform().SetRotation(playerTransform.GetRotation());
	
	//TODO: Make the bullet fire in the direction the player is facing
	XMVECTOR newVel = XMLoadFloat3(&(bulletEntity->GetTransform().GetForwardVector()));
	XMVector3Normalize(newVel);
	XMFLOAT3 velocity;
	XMStoreFloat3(&velocity, XMVector3Normalize(newVel) * 5);
	bulletEntity->GetComponent<PhysicsBody>()->SetVelocity(XMFLOAT4(velocity.x, velocity.y, velocity.z, 0.0f));

	trailParticleEmitter->Enable();
}