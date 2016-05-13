#pragma once
#include "Component.h"
#include "SpriteFont.h"
#include "Render.h"

/*struct RenderTextInfo;
class Render;*/

class ScreenText : public Component
{
public:
	ScreenText(Render* newRender, SpriteFont* newFont, wchar_t* newText);
	~ScreenText();

	void Update(float dt) override;

	void SetColor(XMFLOAT4 newColor);
	void SetText(wchar_t* newText) { text = newText; }
	void SetTextAsInt(int numberAsText);

	void CenterText();

	void Draw(const RenderTextInfo& info);
private:
	Render* render;
	SpriteFont* font;
	XMVECTOR color;
	XMVECTOR pos;
	XMVECTOR offset;
	std::wstring text;
};

