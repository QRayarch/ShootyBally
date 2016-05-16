#pragma once

#include "Entity.h"
#include "DirectXGameCore.h"
#include "PhysicsBody.h"

class Bullet
{
public:
	Bullet();
	Bullet(Entity* entity);
	~Bullet();

	bool GetIsActive() { return isActive; }
	void SetIsActive(bool activity);
	Entity* GetEntity() { return bulletEntity; }

	void Fire(Transform playerTransform);
	void UpdatePhysics(float deltaTime);

private:
	Entity* bulletEntity;

	bool isActive = false;
	float lifespan;
	float lifespanLeft;
};

