#pragma once
#include "Component.h"
#include "Input.h"

class Button : public Component
{
public:
	Button();
	~Button();

	void Update() override;

	void SetOnClicked(void(*newClicked)(void)) { clicked = newClicked; }

private:
	void(*clicked)(void);

	void CheckForMouse();
	void Clicked();
};

