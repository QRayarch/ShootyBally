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
private:
	Entity* playerEntity;
	PlayerNumber playerNum;

	float turnRate;
	float turnDrag;
	float turnSpeed;

	float timeToLastShot;
	float shotTimer;
	Bullet* bulletPool;
};
