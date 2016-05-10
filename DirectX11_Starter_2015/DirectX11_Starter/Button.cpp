#include "Button.h"
#include "Input.h"
#include "Transform.h"

Button::Button()
{
	Component::Component();
	clicked = nullptr;
}


Button::~Button()
{
}

void Button::Update()
{
	CheckForMouse();
}

void Button::CheckForMouse()
{
	//Check to see if the input is there
	if (Input::GetMouseLeftBtnDown()) {
		float mX = Input::GetMousePositionNormalizedX() * 2.0f - 1;//Convert to the range -1 to 1
		float mY = Input::GetMousePositionNormalizedY() * 2.0f - 1;//Convert to the range -1 to 1
		float hW = GetTransform().GetScale().x * 0.5f;
		float hH = GetTransform().GetScale().y * 0.5f;
		float qx = GetTransform().GetPosition().x - hW;
		float wx = GetTransform().GetPosition().x + hW;
		float qy = GetTransform().GetPosition().y - hH;
		float wy = GetTransform().GetPosition().y + hH;
		bool one = GetTransform().GetPosition().x - hW <= mX;
		bool two = GetTransform().GetPosition().x + hW >= mX;
		bool three = GetTransform().GetPosition().y - hH <= mY;
		bool four = GetTransform().GetPosition().y + hH <= mY;
		if (GetTransform().GetPosition().x - hW <= mX && mX <= GetTransform().GetPosition().x + hW &&
			GetTransform().GetPosition().y - hH <= mY && mY <= GetTransform().GetPosition().y + hH) {
			Clicked();
		}
	}
}

void Button::Clicked()
{
	if (clicked != nullptr) {
		(*clicked)();
	}
}
