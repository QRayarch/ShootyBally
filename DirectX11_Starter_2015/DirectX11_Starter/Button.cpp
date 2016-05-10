#include "Button.h"
#include "Input.h"
#include "Transform.h"
#include "Entity.h"

Button::Button()
{
	Component::Component();
	clicked = nullptr;
	isHover = false;
}


Button::~Button()
{
}

void Button::ParentSet()
{
	element = GetEntity()->GetComponent<UIElement>();
}

void Button::Update()
{
	CheckForMouse();
}

void Button::CheckForMouse()
{
	//Check to see if the mouse is in the ui rect
	float mX = Input::GetMousePositionNormalizedX();
	float mY = 1 - Input::GetMousePositionNormalizedY();
	float hW = GetTransform().GetScale().x ;
	float hH = GetTransform().GetScale().y;
	float minX = (GetTransform().GetPosition().x - hW) * 0.5f + 0.5f;
	float maxX = (GetTransform().GetPosition().x + hW) * 0.5f + 0.5f;
	float minY = (GetTransform().GetPosition().y - hH) * 0.5f + 0.5f;
	float maxY = (GetTransform().GetPosition().y + hH) * 0.5f + 0.5f;
	if (minX <= mX && maxX >= mX && minY <= mY && maxY >= mY) {
		if (!isHover) {
			StartHover();
		}
		isHover = true;
	}
	else {
		if (isHover) {
			EndHover();
		}
		isHover = false;
	}
	//Check to see if the input is there
	if (isHover && Input::GetMouseLeftBtnDown()) {
		Clicked();
	}
}

void Button::Clicked()
{
	if (clicked != nullptr) {
		(*clicked)();
	}
}

void Button::StartHover()
{
	if (element != nullptr) {
		element->SetColor(XMFLOAT3(1, 0, 0));
	}
}

void Button::EndHover()
{
	if (element != nullptr) {
		element->SetColor(XMFLOAT3(1, 1, 1));
	}
}
