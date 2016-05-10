#pragma once
#include "Component.h"
#include "UIElement.h"

class Button : public Component
{
public:
	Button();
	~Button();
	void ParentSet() override;

	void Update() override;

	void SetOnClicked(void(*newClicked)(void)) { clicked = newClicked; }

private:
	UIElement* element;
	void(*clicked)(void);

	void CheckForMouse();
	void Clicked();

	void StartHover();
	void EndHover();
	bool isHover;
};

