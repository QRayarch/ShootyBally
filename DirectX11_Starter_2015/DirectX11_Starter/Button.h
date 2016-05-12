#pragma once
#include "Component.h"
#include "UIElement.h"

struct ButtonState {
	XMFLOAT3 color;
	float scale;
	float transitionTime;
};

class Button : public Component
{
public:
	Button();
	~Button();
	void ParentSet() override;

	void Update(float dt) override;

	void SetOnClicked(void(*newClicked)(void)) { clicked = newClicked; }

	void SetButtonStates(ButtonState newDefault, ButtonState newHover) { defaultState = newDefault; hoverState = newHover; }

private:
	XMFLOAT3 oldSize;
	UIElement* element;
	void(*clicked)(void);

	void CheckForMouse();
	void Clicked();

	void StartHover();
	void EndHover();
	bool isHover;
	
	ButtonState* currentState;
	ButtonState* wantedState;
	float currentTime;
	bool hasFinishedTransition;
	void AnimateTransition();

	ButtonState defaultState;
	ButtonState hoverState;
};

