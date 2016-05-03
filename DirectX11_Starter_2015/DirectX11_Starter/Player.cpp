#include "Player.h"
#include "Input.h"

Player::Player()
{
}

Player::Player(EntitySystem* entsys, int entIndex, int playerNumber, Bullet* bullets)
{
	playerEntity = entsys->GetEntity(entIndex);
	turnRate = 8;
	turnDrag = 0.96f;
	turnSpeed = 0.0f;

	timeToLastShot = 2.0f;
	shotTimer = 0.5f;
	bulletPool = bullets;

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
		if (Input::IsKeyDown('P')) { turnSpeed += 0.05f * turnRate * deltaTime; }
		if (Input::IsKeyDown('I')) { turnSpeed -= 0.05f * turnRate * deltaTime; }

		//Firing
		if (Input::IsKeyDown('O') && timeToLastShot >= shotTimer) { Fire(); }
	}
	else if (playerNum == TWO)
	{
		if (Input::IsKeyDown('J')) { turnSpeed += 0.05f * turnRate * deltaTime; }
		if (Input::IsKeyDown('L')) { turnSpeed -= 0.05f * turnRate * deltaTime; }

		//Firing
		if (Input::IsKeyDown('K') && timeToLastShot >= shotTimer) { Fire(); }
	}

	turnSpeed *= turnDrag;
	curRot.y += turnSpeed;
	playerEntity->GetTransform().SetRotation(curRot);
	timeToLastShot += deltaTime;
}

void Player::Fire()
{
	for (int i = 0; i < 20; i++)
	{
		//Fire first inactive bullet
		if (!bulletPool[i].GetIsActive())
		{
			bulletPool[i].Fire(playerEntity->GetTransform());
			break;
		}
	}
	timeToLastShot = 0.0f;
}

void Player::ResetPlayer()
{
	score = 0;
	
}
