#pragma once
#include "UIElement.h"
#include "Button.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "Resources.h"



class Canvas
{
public:
	const static unsigned int MAX_NUMBER_UI_ELEMENTS = 100;

	Canvas(EntitySystem* newEntSys, Render* newRender, Resources* newRes);
	~Canvas();

	void SetAspectRatio(float aspectRatio);

	Button* AddButton(UIRect rect, Material* mat);
	Button* AddButton(UIRect rect, Mesh* mesh, Material* mat);

	ButtonState& GetDefualtButtonState() { return buttonDefaultState; }
	void SetDefaultButtonState(ButtonState newDefault) { buttonDefaultState = newDefault; }
	ButtonState& GetHoverButtonState() { return buttonHoverState; }
	void SetHoverButtonState(ButtonState newHover) { buttonHoverState = newHover; }
private:
	EntitySystem* entSys;
	Render* render;
	Resources* res;
	float aspectRatio;

	Mesh* defaultUIMesh;

	UIElement* uiElements[MAX_NUMBER_UI_ELEMENTS];
	int numUIElements;

	void AddUIElement(UIElement* newElement);

	ButtonState buttonDefaultState;
	ButtonState buttonHoverState;
};

