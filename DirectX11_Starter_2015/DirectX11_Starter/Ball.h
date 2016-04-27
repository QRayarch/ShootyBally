#pragma once

#include "Entity.h"
#include "EntitySystem.h"
#include "DirectXGameCore.h"
#include "PhysicsBody.h"

using namespace DirectX;

class Ball
{
public:
	Ball();
	~Ball();

	void AddForce(float magX, float magY);
	Entity* GetEntity() { return entity; };

private:
	Entity* entity;
};

