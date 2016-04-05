#pragma once

#include "Entity.h"
#include "EntitySystem.h"
#include "DirectXGameCore.h"
#include "PhysicsBody.h"

class Bullet
{
public:
	Bullet();
	Bullet(EntitySystem* entsys, int entIndex);
	~Bullet();

	bool GetIsActive();
	Entity* GetEntity();

	void Fire(Transform playerTransform);
	void UpdatePhysics(float deltaTime);

private:
	Entity* bulletEntity;

	bool isActive = false;
	float lifespan;
	float lifespanLeft;
	float velocity;
};

