#include "Button.h"
#include "Input.h"
#include "Transform.h"
#include "Entity.h"

Button::Button()
{
	Component::Component();
	clicked = nullptr;
	isHover = false;

	currentTime = 0;
	currentState = nullptr;
	wantedState = nullptr;
}


Button::~Button()
{
}

void Button::ParentSet()
{
	element = GetEntity()->GetComponent<UIElement>();
}

void Button::Update(float dt)
{
	currentTime += dt;
	CheckForMouse();
	AnimateTransition();
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
	if (wantedState == nullptr || currentTime / wantedState->transitionTime >= 1.0f) {
		oldSize = GetTransform().GetScale();
	}
	currentTime = 0;
	hasFinishedTransition = false;
	currentState = &defaultState;
	wantedState = &hoverState;
}

void Button::EndHover()
{

	currentTime = 0;
	hasFinishedTransition = false;
	currentState = &hoverState;
	wantedState = &defaultState;
}

void Button::AnimateTransition()
{
	if (element == nullptr || wantedState == nullptr || currentState == nullptr) return;
	float normal = currentTime / wantedState->transitionTime;
	
	if (normal <= 1.0f) {
		XMVECTOR currentColor = XMLoadFloat3(&currentState->color);
		XMVECTOR wantedColor = XMLoadFloat3(&wantedState->color);
		XMFLOAT3 finalColor;
		XMStoreFloat3(&finalColor, XMVectorLerp(currentColor, wantedColor, normal));
		element->SetColor(finalColor);

		XMVECTOR scale = XMLoadFloat3(&oldSize);
		XMVECTOR currentScale = scale * currentState->scale;
		XMVECTOR wantedScale = scale * wantedState->scale;
		XMFLOAT3 finalScale;
		XMStoreFloat3(&finalScale, XMVectorLerp(currentScale, wantedScale, normal));
		element->SetScale(finalScale.x, finalScale.y);
	}
	else {
		if (!hasFinishedTransition) {
			element->SetColor(wantedState->color);
			XMVECTOR scale = XMLoadFloat3(&oldSize) * wantedState->scale;
			XMFLOAT3 finalScale;
			XMStoreFloat3(&finalScale, scale);
			element->SetScale(finalScale.x, finalScale.y);
		}
		hasFinishedTransition = true;
	}
}
