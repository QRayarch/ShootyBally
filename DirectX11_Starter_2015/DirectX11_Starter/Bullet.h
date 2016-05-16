#pragma once

#include "Entity.h"
#include "DirectXGameCore.h"
#include "PhysicsBody.h"
#include "ParticleEmitter.h"

class Bullet
{
public:
	Bullet();
	Bullet(Entity* entity, ParticleEmitter* trailParticleEmitter);
	~Bullet();

	bool GetIsActive() { return isActive; }
	void SetIsActive(bool activity);
	Entity* GetEntity() { return bulletEntity; }

	void Fire(Transform playerTransform);
	void UpdatePhysics(float deltaTime);

private:
	Entity* bulletEntity;
	ParticleEmitter* trailParticleEmitter;

	bool isActive = false;
	float lifespan;
	float lifespanLeft;
	float velocity;
};

