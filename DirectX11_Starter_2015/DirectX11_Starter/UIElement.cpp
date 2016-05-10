#include "UIElement.h"



UIElement::UIElement(Render* render, Mesh* mesh, Material* material)
{
	drawnMesh = DrawnMesh(render, mesh, material);
	color = DirectX::XMFLOAT3(1, 1, 1);
}


UIElement::~UIElement()
{
	drawnMesh.~DrawnMesh();
}

void UIElement::Update()
{

	//drawnMesh.GetMaterial()->GetVertexMaterialInfo()->GetFloat3(0)->data = GetTransform().GetScale();
	drawnMesh.Update();
}

void UIElement::ParentSet()
{
	drawnMesh.SetEntity(GetEntity());
}

void UIElement::SetColor(DirectX::XMFLOAT3 newColor)
{
	color = newColor;
	//drawnMesh.GetIndividualVertexInfo()->GetFloat3(1)->data = color;
}

void UIElement::SetAspectRatio(float aspectRatio)
{
	//drawnMesh.GetMaterial()->GetPixelMaterialInfo().GetFloat(0).data = aspectRatio;
}

void UIElement::SetRect(UIRect rect)
{
	GetTransform().SetPosition(DirectX::XMFLOAT3(rect.x, rect.y, 0));
	GetTransform().SetScale(DirectX::XMFLOAT3(rect.w, rect.h, 1));//Set the proper position
}
