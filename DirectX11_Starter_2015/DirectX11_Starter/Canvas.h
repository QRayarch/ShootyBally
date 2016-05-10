#pragma once
#include "UIElement.h"
#include "Button.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "Resources.h"

#define ASPECT_RATIO_SHADER_INDEX 5
#define SCALE_SHADER_INDEX 3
#define COLOR_SHADER_INDEX 4

class Canvas
{
public:
	const static unsigned int MAX_NUMBER_UI_ELEMENTS = 100;

	Canvas(EntitySystem* newEntSys, Render* newRender, Resources* newRes);
	~Canvas();

	void SetAspectRatio(float aspectRatio);

	Button* AddButton(UIRect rect, Material* mat);
	Button* AddButton(UIRect rect, Mesh* mesh, Material* mat);
private:
	EntitySystem* entSys;
	Render* render;
	Resources* res;
	float aspectRatio;

	Mesh* defaultUIMesh;

	UIElement* uiElements[MAX_NUMBER_UI_ELEMENTS];
	int numUIElements;

	void AddUIElement(UIElement* newElement);
};

