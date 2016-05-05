#pragma once
#include "DrawnMesh.h"
#include "Component.h"

class UIElement : public Component
{
public:
	UIElement(Render* render, Mesh* mesh, Material* material);
	~UIElement();

	void Update() override;
	void ParentSet() override;
	void SetColor(DirectX::XMFLOAT3 newColor);
	void SetAspectRatio(float aspectRatio);
private:

	DrawnMesh drawnMesh;
	DirectX::XMFLOAT3 color;
};

