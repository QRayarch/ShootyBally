#include "CollisionCircle.h"



CollisionCircle::CollisionCircle(Vertex* meshVerts, int numVertices)
{
	modelMatrix = XMFLOAT4X4();

	radius = meshVerts[0].Position.z;
	for (int i = 0; i < numVertices; i++)
	{
		if (abs(radius) < abs(meshVerts[i].Position.z))
		{
			radius = meshVerts[i].Position.z;
		}
	}
}

CollisionCircle::~CollisionCircle()
{
}

void CollisionCircle::UpdateCollisionCircle(Entity* entity)
{
	modelMatrix = entity->GetTransform().GetWorldMatrix();
	scale = entity->GetTransform().GetScale().x;
}

bool CollisionCircle::IsColliding(CollisionCircle* collider)
{
	XMFLOAT3 distanceVec;
	XMStoreFloat3(&distanceVec, XMLoadFloat3(&center) - XMLoadFloat3(&collider->GetCenter()));
	float distance = sqrt((distanceVec.x * distanceVec.x) + (distanceVec.z * distanceVec.z));

	return distance < radius * scale + collider->GetRadius();
}