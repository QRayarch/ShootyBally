#include "Player.h"
#include "Input.h"

Player::Player()
{
	score = 0;
}

Player::Player(Entity* entity, int playerNumber, ParticleEmitter* goalParticleEmitter, ParticleEmitter* goalExplosionParticleEmitter, Bullet* bullets)
{
	playerEntity = entity;
	this->goalParticleEmitter = goalParticleEmitter;
	this->goalExplosionParticleEmitter = goalExplosionParticleEmitter;
	turnRate = 8;
	turnDrag = 0.96f;
	turnSpeed = 0.0f;

	timeToLastShot = 2.0f;
	shotTimer = 0.5f;
	bulletPool = bullets;

	playerNum = PlayerNumber(playerNumber);
	score = 0;
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
		if (Input::IsKeyDown('D')) { turnSpeed += 0.05f * turnRate * deltaTime; }
		if (Input::IsKeyDown('A')) { turnSpeed -= 0.05f * turnRate * deltaTime; }

		//Firing
		if (Input::IsKeyDown('W') && timeToLastShot >= shotTimer) { Fire(); }
	}
	else if (playerNum == TWO)
	{
		if (Input::IsKeyDown(VK_RIGHT)) { turnSpeed += 0.05f * turnRate * deltaTime; }
		if (Input::IsKeyDown(VK_LEFT)) { turnSpeed -= 0.05f * turnRate * deltaTime; }

		//Firing
		if (Input::IsKeyDown(VK_UP) && timeToLastShot >= shotTimer) { Fire(); }
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

void Player::EnableGoalParticles()
{
	goalParticleEmitter->Enable();
	goalExplosionParticleEmitter->Enable();
	goalExplosionParticleEmitter->Disable();
}

void Player::DisableGoalParticles()
{
	goalParticleEmitter->Disable();
}
