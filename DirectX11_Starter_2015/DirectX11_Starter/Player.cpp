#include "Player.h"

Player::Player()
{
}

Player::Player(EntitySystem* entsys, int entIndex, int playerNumber)
{
	playerEntity = entsys->GetEntity(entIndex);
	turnRate = 8;
	turnDrag = 0.96f;
	turnSpeed = 0.0f;

	timeToLastShot = 2.0f;
	shotTimer = 2.0f;

	playerNum = PlayerNumber(playerNumber);
}

Player::~Player()
{
}

void Player::GetInput(float deltaTime)
{
	XMFLOAT3 curRot = playerEntity->GetTransform().GetRotation();

	if (playerNum == ONE)
	{
		//Movement
		if (GetAsyncKeyState('P') & 0x8000) { turnSpeed += 0.05f * turnRate * deltaTime; }
		if (GetAsyncKeyState('I') & 0x8000) { turnSpeed -= 0.05f * turnRate * deltaTime; }

		//Firing
		if (GetAsyncKeyState('O') & 0x8000 && timeToLastShot >= shotTimer) { Fire(); }
	}
	else if (playerNum == TWO)
	{
		if (GetAsyncKeyState('J') & 0x8000) { turnSpeed += 0.05f * turnRate * deltaTime; }
		if (GetAsyncKeyState('L') & 0x8000) { turnSpeed -= 0.05f * turnRate * deltaTime; }

		//Firing
		if (GetAsyncKeyState('K') & 0x8000 && timeToLastShot >= shotTimer) { Fire(); }
	}

	turnSpeed *= turnDrag;
	curRot.x += turnSpeed;
	playerEntity->GetTransform().SetRotation(curRot);
	timeToLastShot += deltaTime;
}

void Player::Fire()
{
	//Shoot Bullet
	timeToLastShot = 0.0f;
}