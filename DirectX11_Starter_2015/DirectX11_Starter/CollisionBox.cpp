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

	innerRadius = std::fminf(halfWidth, halfLength);
}


CollisionBox::~CollisionBox()
{
	Component::Update();
	UpdateCollisionBox();
}

void CollisionBox::UpdateCollisionBox()
{
	modelMatrix = GetEntity()->GetTransform().GetWorldMatrix();
	scale = GetEntity()->GetTransform().GetScale().x;
}

bool CollisionBox::IsColliding(CollisionCircle* collider)
{
	//Check if within inner radius
	XMFLOAT3 distanceVec;
	XMStoreFloat3(&distanceVec, XMLoadFloat3(&centroid) - XMLoadFloat3(&collider->GetCenter()));
	float distance = sqrt((distanceVec.x * distanceVec.x) + (distanceVec.z * distanceVec.z));
	if (distance < innerRadius * scale + collider->GetRadius())
		return true;

	XMFLOAT3 centerToCenterVector;
	XMStoreFloat3(&centerToCenterVector, XMLoadFloat3(&collider->GetCenter()) - XMLoadFloat3(&centroid));
	centerToCenterVector.y = 0;
	XMStoreFloat3(&centerToCenterVector, XMVector3Normalize(XMLoadFloat3(&centerToCenterVector)));

	XMFLOAT3 outerPoint;
	XMStoreFloat3(&outerPoint, XMLoadFloat3(&collider->GetCenter()) + XMLoadFloat3(&centerToCenterVector) * collider->GetRadius());

	//SAT Check
	//Calculate Normals
	XMFLOAT3 normals[2];
	XMMATRIX modelMatrixL = XMLoadFloat4x4(&modelMatrix);
	XMStoreFloat3(&normals[0], XMVector3Normalize(XMVector4Transform(XMLoadFloat4(&XMFLOAT4(1, 0, 0, 1)), modelMatrixL) - XMVector4Transform(XMLoadFloat4(&XMFLOAT4(0, 0, 0, 1)), modelMatrixL)));
	XMStoreFloat3(&normals[1], XMVector3Normalize(XMVector4Transform(XMLoadFloat4(&XMFLOAT4(0, 0, 1, 1)), modelMatrixL) - XMVector4Transform(XMLoadFloat4(&XMFLOAT4(0, 0, 0, 1)), modelMatrixL)));

	//Axis 1
	XMFLOAT3 maxG;
	XMStoreFloat3(&maxG, XMVector3Transform(XMLoadFloat3(&maxVert), modelMatrixL));
	XMFLOAT3 minG;
	XMStoreFloat3(&minG, XMVector3Transform(XMLoadFloat3(&minVert), modelMatrixL));

	XMFLOAT2 dotA;
	XMStoreFloat2(&dotA, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(normals[0].x, normals[0].z)), XMLoadFloat2(&XMFLOAT2(maxG.x, maxG.z))));
	XMFLOAT2 dotB;
	XMStoreFloat2(&dotA, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(normals[0].x, normals[0].z)), XMLoadFloat2(&XMFLOAT2(minG.x, minG.z))));
	XMFLOAT2 dotO;
	XMStoreFloat2(&dotA, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(normals[0].x, normals[0].z)), XMLoadFloat2(&XMFLOAT2(outerPoint.x, outerPoint.z))));

	float dotMin, dotMax, dotOV;
	dotOV = dotO.x;
	if (dotA.x > dotB.x) {
		dotMin = dotB.x;
		dotMax = dotA.x;
	}
	else {
		dotMin = dotA.x;
		dotMax = dotB.x;
	}
	if (dotMin > dotOV || dotMax < dotOV)
		return false;


	//Axis 2
	XMStoreFloat2(&dotA, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(normals[1].x, normals[1].z)), XMLoadFloat2(&XMFLOAT2(maxG.x, maxG.z))));
	XMStoreFloat2(&dotA, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(normals[1].x, normals[1].z)), XMLoadFloat2(&XMFLOAT2(minG.x, minG.z))));
	XMStoreFloat2(&dotA, XMVector2Dot(XMLoadFloat2(&XMFLOAT2(normals[1].x, normals[1].z)), XMLoadFloat2(&XMFLOAT2(outerPoint.x, outerPoint.z))));

	dotOV = dotO.x;
	if (dotA.x > dotB.x) {
		dotMin = dotB.x;
		dotMax = dotA.x;
	}
	else {
		dotMin = dotA.x;
		dotMax = dotB.x;
	}
	if (dotMin > dotOV || dotMax < dotOV)
		return false;
	//If both fail, should be colliding
	return true;
}
