#include "CollisionBox.h"
#include "DebugDraw.h"


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
	
}

void CollisionBox::Update()
{
	Component::Update();
	modelMatrix = GetEntity()->GetTransform().GetWorldMatrix();
	scale = GetEntity()->GetTransform().GetScale().x;
	DebugDraw::AddBox(GetEntity()->GetTransform().GetPosition(), GetEntity()->GetTransform().GetScale(), DirectX::XMFLOAT4(1, 1, 1, 1));
}

bool CollisionBox::IsColliding(CollisionCircle* collider)
{
	XMMATRIX modelMatrixL = XMLoadFloat4x4(&modelMatrix);
	XMVECTOR centroidG = XMVector3Transform(XMLoadFloat3(&centroid), modelMatrixL);
	//Check if within inner radius
	XMFLOAT3 distanceVec;
	XMStoreFloat3(&distanceVec, centroidG - XMLoadFloat3(&collider->GetCenter()));
	float distance = sqrt((distanceVec.x * distanceVec.x) + (distanceVec.z * distanceVec.z));
	if (distance < innerRadius * scale + collider->GetRadius())
		return true;

	XMFLOAT3 centerToCenterVector;
	XMStoreFloat3(&centerToCenterVector, XMLoadFloat3(&collider->GetCenter()) - centroidG);
	centerToCenterVector.y = 0;
	XMStoreFloat3(&centerToCenterVector, XMVector3Normalize(XMLoadFloat3(&centerToCenterVector)));

	XMFLOAT3 outerPoint;
	XMStoreFloat3(&outerPoint, XMLoadFloat3(&collider->GetCenter()) + XMLoadFloat3(&centerToCenterVector) * collider->GetRadius());

	//SAT Check
	//Calculate Normals
	XMFLOAT3 normals[2];
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



	//////not sure if this will work....
	//XMMATRIX modelMatrixL = XMLoadFloat4x4(&modelMatrix);
	//XMFLOAT3 boxToCircle;
	//XMFLOAT3 boxCenter;
	//XMStoreFloat3(&boxCenter, XMVector3Transform(XMLoadFloat3(&centroid), modelMatrixL));
	//XMStoreFloat3(&boxToCircle, XMLoadFloat3(&boxCenter) - XMLoadFloat3(&collider->GetCenter()));
	//XMFLOAT3 closestCorner;

	//float max = std::numeric_limits<float>::min();
	//var box2circle : Vector2d = new Vector2d(c.x - center_box.x, c.y - center_box.y)
	//	var box2circle_normalised : Vector2d = box2circle.unitVector

	//	//get the maximum
	//	for (var i : int = 1; i < 5; i++)
	//	{
	//		current_box_corner = box1.getDot(i)
	//			v = new Vector2d(
	//				current_box_corner.x - center_box.x,
	//				current_box_corner.y - center_box.y);
	//		var current_proj : Number = v.dotProduct(box2circle_normalised)

	//			if (max < current_proj) max = current_proj;
	//	}
	//if (box2circle.magnitude - max - c.radius > 0 && box2circle.magnitude > 0) t.text = "No Collision"
	//else t.text = "Collision"
}
