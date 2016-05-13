#pragma once
#include "Component.h"
#include "PhysicsBody.h"

class VelocityRotator :
	public Component
{
public:
	VelocityRotator(float scale);
	~VelocityRotator();

	void ParentSet() override;
	void Update(float dt) override;
private:
	PhysicsBody* body;
	float speedScale;
};

