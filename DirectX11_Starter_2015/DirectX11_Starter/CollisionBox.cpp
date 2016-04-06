#include "CollisionBox.h"



CollisionBox::CollisionBox(Vertex* meshVertices, int numVerts)
{
	vertices = meshVertices;
	numVertices = numVerts;
	modelMatrix = XMFLOAT4X4();
	
	minVert = maxVert = vertices[0].Position;

	for (int i = 0; i < numVertices; i++)
	{
		if (vertices[i].Position.x < minVert.x)
			minVert.x = vertices[i].Position.x;
		else if (vertices[i].Position.x > maxVert.x)
			maxVert.x = vertices[i].Position.x;
		if (vertices[i].Position.z < minVert.z)
			minVert.z = vertices[i].Position.z;
		else if (vertices[i].Position.z > maxVert.z)
			maxVert.z = vertices[i].Position.z;
	}

	centroid.x = (minVert.x + maxVert.x) / 2;
	centroid.z = (minVert.z + maxVert.z) / 2;

	halfWidth = maxVert.x - minVert.x;
	halfLength = maxVert.z - minVert.z;
}


CollisionBox::~CollisionBox()
{
}

void CollisionBox::UpdateCollisionBox(Entity* entity)
{
	modelMatrix = entity->GetTransform().GetWorldMatrix();
}

void CollisionBox::CheckCollisions(CollisionCircle* collider)
{
}
