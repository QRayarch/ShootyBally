#pragma once

#include "Entity.h"
#include "EntitySystem.h"
#include "DirectXGameCore.h"
#include "Bullet.h"

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
	Player(EntitySystem* entsys, int entIndex, int playerNumber, Bullet* bullets);
	~Player();

	void GetInput(float deltaTime);
	void Fire();

	//CollisionCircle* GetCircleCollider() { return playerEntity->GetComponent<CollisionCircle>(); }
	//CollisionBox* GetBoxCollider() { return playerEntity->GetComponent<CollisionBox>(); }

	short GetScore() { return score; };
	void AddPoint() { ++score; };
	void ResetPlayer();

private:
	Entity* playerEntity;
	PlayerNumber playerNum;

	float turnRate;
	float turnDrag;
	float turnSpeed;

	float timeToLastShot;
	float shotTimer;
	Bullet* bulletPool;

	short score;
};

