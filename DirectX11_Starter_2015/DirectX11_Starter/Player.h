#pragma once

#include "Entity.h"
#include "DirectXGameCore.h"
#include "Bullet.h"
#include "CollisionCircle.h"
#include "CollisionBox.h"
#include "PhysicsBody.h"

using namespace DirectX;

enum PlayerNumber
{
	ONE = 1,
	TWO = 2
};

class Player
{
public:
	Player();
	Player(Entity* entity, int playerNumber, ParticleEmitter* goalParticleEmitter, ParticleEmitter* goalExplosionParticleEmitter, Bullet* bullets);
	~Player();

	void GetInput(float deltaTime);
	void Fire();

	CollisionCircle* GetCircleCollider() { return playerEntity->GetComponent<CollisionCircle>(); }
	CollisionBox* GetBoxCollider() { return playerEntity->GetComponent<CollisionBox>(); }

	PhysicsBody* GetPlayerPB() { return playerEntity->GetComponent<PhysicsBody>(); }

	short GetScore() { return score; }
	void AddPoint() { ++score; }
	void ResetPlayer();

	void EnableGoalParticles();
	void DisableGoalParticles();

private:
	Entity* playerEntity;
	PlayerNumber playerNum;

	ParticleEmitter* goalParticleEmitter;
	ParticleEmitter* goalExplosionParticleEmitter;

	float turnRate;
	float turnDrag;
	float turnSpeed;

	float timeToLastShot;
	float shotTimer;
	Bullet* bulletPool;

	short score;
};

