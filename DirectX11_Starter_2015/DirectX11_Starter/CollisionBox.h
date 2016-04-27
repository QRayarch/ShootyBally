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

	void UpdateCollisionBox();
	bool IsColliding(CollisionCircle* collider);

	XMFLOAT4X4 GetModelMatrix() { return modelMatrix; }

private:
	int numVertices;
	float innerRadius;
	float scale;
	float halfWidth, halfLength;
	XMFLOAT3 minVert, maxVert, centroid;
	XMFLOAT4X4 modelMatrix;
	Vertex* vertices;
};

