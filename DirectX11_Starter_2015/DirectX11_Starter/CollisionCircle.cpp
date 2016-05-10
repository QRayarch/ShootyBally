#include "CollisionCircle.h"
#include "DebugDraw.h"
#include "Logger.h"


CollisionCircle::CollisionCircle(Vertex* meshVerts, int numVertices)
{
	Component::Component();
	radius = abs(meshVerts[0].Position.x);
	for (int i = 0; i < numVertices; i++)
	{
		if (abs(radius) < abs(meshVerts[i].Position.x))
		{
			radius = abs(meshVerts[i].Position.x);
		}
	}
	center = XMFLOAT3(0, 0, 0);
}

CollisionCircle::~CollisionCircle()
{
}

void CollisionCircle::Update()
{
	Component::Update();
	UpdateCollisionCircle();
}

void CollisionCircle::UpdateCollisionCircle()
{
	scale = GetEntity()->GetTransform().GetScale().x;
	center = GetEntity()->GetTransform().GetPosition();
	DebugDraw::AddSphere(center, GetRadius(), DirectX::XMFLOAT4(1, 1, 1, 1));
}

bool CollisionCircle::IsColliding(CollisionCircle* collider)
{
	XMFLOAT3 distanceVec;
	XMStoreFloat3(&distanceVec, XMLoadFloat3(&center) - XMLoadFloat3(&collider->GetCenter()));
	float distance = sqrt((distanceVec.x * distanceVec.x) + (distanceVec.z * distanceVec.z));

	return distance < radius * scale + collider->GetRadius();
}