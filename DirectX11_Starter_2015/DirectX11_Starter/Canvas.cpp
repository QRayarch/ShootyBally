#include "Canvas.h"



Canvas::Canvas(EntitySystem* newEntSys, Render* newRender, Resources* newRes)
{
	entSys = newEntSys;
	render = newRender;
	res = newRes;
	numUIElements = 0;
	aspectRatio = 1;

	//Set up dedault states
	buttonDefaultState = { XMFLOAT3(1, 1, 1), 1, 0 };
	buttonHoverState = { XMFLOAT3(1, 0, 0), 1, 0 };

	defaultUIMesh = res->GetMeshIfLoaded("DefaultUIElement");
	//If we haven't found the default ui mesh generate one
	if (defaultUIMesh == nullptr) {
		float halfSize = 1.0f;
		float boarderSize = 0.016f;
		XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
		XMFLOAT3 innerVertex = XMFLOAT3(1, 1, 1);//Used to denote the inver vertex for proper scaling
		XMFLOAT3 tangent = XMFLOAT3(0, 0, 0);
		Vertex* vertices2 = new Vertex[16];
		//Top Row
		vertices2[0] = { XMFLOAT3(-halfSize, halfSize, 0), normal, XMFLOAT2(0, 1), tangent };
		vertices2[1] = { XMFLOAT3(-halfSize + boarderSize, halfSize, 0), normal, XMFLOAT2(boarderSize, 1), tangent };
		vertices2[2] = { XMFLOAT3(+halfSize - boarderSize, halfSize, 0), normal, XMFLOAT2(1 - boarderSize, 1), tangent };
		vertices2[3] = { XMFLOAT3(halfSize, halfSize, 0), normal, XMFLOAT2(1, 1), tangent };

		//Top Lower Row
		vertices2[4] = { XMFLOAT3(-halfSize, halfSize - boarderSize, 0), normal, XMFLOAT2(0, 1 - boarderSize), tangent };
		vertices2[5] = { XMFLOAT3(-halfSize + boarderSize, halfSize - boarderSize, 0), innerVertex, XMFLOAT2(boarderSize, 1 - boarderSize), tangent };
		vertices2[6] = { XMFLOAT3(+halfSize - boarderSize, halfSize - boarderSize, 0), innerVertex, XMFLOAT2(1 - boarderSize, 1 - boarderSize), tangent };
		vertices2[7] = { XMFLOAT3(halfSize, halfSize - boarderSize, 0), normal, XMFLOAT2(1, 1 - boarderSize), tangent };

		//Bottom Upper Row
		vertices2[8] = { XMFLOAT3(-halfSize, -halfSize + boarderSize, 0), normal, XMFLOAT2(0, boarderSize), tangent };
		vertices2[9] = { XMFLOAT3(-halfSize + boarderSize, -halfSize + boarderSize, 0), innerVertex, XMFLOAT2(boarderSize, boarderSize), tangent };
		vertices2[10] = { XMFLOAT3(+halfSize - boarderSize, -halfSize + boarderSize, 0), innerVertex, XMFLOAT2(1 - boarderSize, boarderSize), tangent };
		vertices2[11] = { XMFLOAT3(halfSize, -halfSize + boarderSize, 0), normal, XMFLOAT2(1, boarderSize), tangent };

		//Bottom Row
		vertices2[12] = { XMFLOAT3(-halfSize, -halfSize, 0), normal, XMFLOAT2(0, 0), tangent };
		vertices2[13] = { XMFLOAT3(-halfSize + boarderSize, -halfSize, 0), normal, XMFLOAT2(boarderSize, 0), tangent };
		vertices2[14] = { XMFLOAT3(+halfSize - boarderSize, -halfSize, 0), normal, XMFLOAT2(1 - boarderSize, 0), tangent };
		vertices2[15] = { XMFLOAT3(halfSize, -halfSize, 0), normal, XMFLOAT2(1, 0), tangent };
		UINT indices2[] = { 0, 1, 4, 1, 5, 4, 1, 2, 5, 2, 6, 5, 2, 3, 6, 3, 7, 6,
			4, 5, 8, 5, 9, 8, 5, 6, 9, 6, 10, 9, 6, 7, 10, 7, 11, 10,
			8, 9, 12, 9, 13, 12, 9, 10, 13, 10, 14, 13, 10, 11, 14, 11, 15, 14 };
		defaultUIMesh = res->AddMesh("DefaultUIElement", vertices2, 16, indices2, 54);
	}
}


Canvas::~Canvas()
{
}

void Canvas::SetAspectRatio(float newAspectRatio)
{
	aspectRatio = newAspectRatio;
	for (int u = 0; u < numUIElements; ++u) {
		uiElements[u]->SetAspectRatio(newAspectRatio);
	}
}

Button * Canvas::AddButton(UIRect rect, Material * mat)
{
	return AddButton(rect, defaultUIMesh, mat);
}

Button * Canvas::AddButton(UIRect rect, Mesh* mesh, Material* mat)
{
	if (entSys->CanAddEntity()) {
		//Set the material to handle UI
		//Dont have a property for it add one

		//Create the Entity
		Entity* newEnt = entSys->AddEntity();
		UIElement* newElement = new UIElement(render, mesh, mat);
		newEnt->AddComponent(newElement);
		newElement->SetRect(rect);
		newElement->SetAspectRatio(aspectRatio);

		AddUIElement(newElement);
		Button* newButton = new Button();
		newEnt->AddComponent(newButton);
		newButton->SetButtonStates(buttonDefaultState, buttonHoverState);

		return newButton;
	}
	return nullptr;
}

void Canvas::AddUIElement(UIElement * newElement)
{
	if (numUIElements < MAX_NUMBER_UI_ELEMENTS) {
		uiElements[numUIElements] = newElement;
		numUIElements += 1;
	}
}
