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

	void UpdateCollisionCircle(Entity* entity);
	bool IsColliding(CollisionCircle* collider);

	float GetRadius() { return radius; }
	XMFLOAT3 GetCenter() { return center; }
private:
	float radius;
	XMFLOAT4X4 modelMatrix;
	XMFLOAT3 center;
};

