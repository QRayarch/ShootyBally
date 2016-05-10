#include "Button.h"



Button::Button()
{
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
		float x = Input::GetMousePositionNormalizedX() * 2 - 1;//Convert to the range -1 to 1
		float y = Input::GetMousePositionNormalizedY() * 2 - 1;//Convert to the range -1 to 1
		Clicked();
	}
}

void Button::Clicked()
{
	if (clicked != nullptr) {
		(*clicked)();
	}
}
