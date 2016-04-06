#pragma once

#include "Component.h"
#include "Vertex.h"
#include "Entity.h"
#include "CollisionCircle.h"
#include <DirectXMath.h>

using namespace DirectX;

class CollisionBox : public Component
{
public:
	CollisionBox(Vertex* meshVertices, int numVerts);
	~CollisionBox();

	void UpdateCollisionBox(Entity* entity);

	void CheckCollisions(CollisionCircle* collider);
private:
	Vertex* vertices;
	int numVertices;
	XMFLOAT3 minVert, maxVert, centroid;
	float halfWidth, halfLength;
	XMFLOAT4X4 modelMatrix;
};

