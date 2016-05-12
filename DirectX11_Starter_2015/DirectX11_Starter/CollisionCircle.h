#pragma once

#include "Component.h"
#include "Vertex.h"
#include "Entity.h"
#include <DirectXMath.h>

using namespace DirectX;

class CollisionCircle : public Component
{
public:
	CollisionCircle(Vertex* meshVerts, int numVertices);
	~CollisionCircle();

	void Update(float dt) override;

	void UpdateCollisionCircle();
	bool IsColliding(CollisionCircle* collider);

	float GetRadius() { return radius * scale; }
	XMFLOAT3 GetCenter() { return center; }
private:
	float radius;
	float scale;

	XMFLOAT3 center;
};

