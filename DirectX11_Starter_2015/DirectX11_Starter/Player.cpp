#include "Player.h"

Player::Player()
{
}

Player::Player(EntitySystem* entsys, int entIndex, int playerNumber)
{
	playerEntity = entsys->GetEntity(entIndex);
	turnRate = 28;
	turnDrag = 1.0f;
	turnSpeed = 0.9f;

	playerNum = PlayerNumber(playerNumber);
}

Player::~Player()
{
}

void Player::GetInput()
{
	XMFLOAT3 curRot = playerEntity->GetTransform().GetRotation();

	if (playerNum == ONE)
	{
		if (GetAsyncKeyState('O') & 0x8000) { curRot.x += 0.05f; }
		if (GetAsyncKeyState('I') & 0x8000) { curRot.x -= 0.05f; }
	}
	else if (playerNum == TWO)
	{
		if (GetAsyncKeyState('K') & 0x8000) { curRot.x += 0.05f; }
		if (GetAsyncKeyState('L') & 0x8000) { curRot.x -= 0.05f; }
	}
	playerEntity->GetTransform().SetRotation(curRot);
}