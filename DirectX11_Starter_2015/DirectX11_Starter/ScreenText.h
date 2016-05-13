#pragma once
#include "Component.h"
#include "SpriteFont.h"

struct RenderTextInfo;
class Render;

class ScreenText : public Component
{
public:
	ScreenText();
	~ScreenText();

	void Update(float dt) override;

	void Draw(RenderTextInfo info);
private:
	Render* render;
//	SpriteFont* font;
};

