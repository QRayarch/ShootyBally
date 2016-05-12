#pragma once
#include "DrawnMesh.h"
#include "Component.h"

struct UIRect {
	float x, y;
	float w, h;
};

#define ASPECT_RATIO_SHADER_INDEX 5
#define SCALE_SHADER_INDEX 3
#define COLOR_SHADER_INDEX 4

class UIElement : public DrawnMesh
{
public:
	UIElement(Render* render, Mesh* mesh, Material* material);
	~UIElement();

	void SetColor(DirectX::XMFLOAT3 newColor);
	void SetAspectRatio(float aspectRatio);

	void SetRect(UIRect rect);
	void SetScale(float x, float y);
private:
	DirectX::XMFLOAT3 color;
};

