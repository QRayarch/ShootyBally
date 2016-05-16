#include "ScreenText.h"

ScreenText::ScreenText(Render* newRender, SpriteFont* newFont, wchar_t* newText)
	: Component()
{
	render = newRender;
	font = newFont;
	text = newText;

	color = XMFLOAT4(1, 1, 1, 1);
}


ScreenText::~ScreenText()
{
}

void ScreenText::Update(float dt)
{
	if (render == nullptr) return;
	render->AddTextToRender(*this);
}

void ScreenText::SetColor(XMFLOAT4 newColor)
{
	color = newColor;
}

void ScreenText::SetTextAsInt(int numberAsText)
{
	text = std::to_wstring(numberAsText);
}

void ScreenText::CenterText()
{
	XMStoreFloat4(&offset, font->MeasureString(text.c_str()) * -0.5);
}

void ScreenText::Draw(const RenderTextInfo& info)
{
	XMFLOAT3 newPos = GetTransform().GetPosition();
	newPos.y = 1 - (newPos.y * 0.5 + 0.5);
	XMVECTOR screenPos = XMLoadFloat3(&newPos) * XMVectorSet(0.5, 1, 0, 0) + XMVectorSet(0.5, 0, 0, 0);
	font->DrawString(info.spriteBatch, text.c_str(), screenPos * info.screenSize + XMLoadFloat4(&offset), XMLoadFloat4(&color));//screenPos * info.screenSize
}
