#include "UIElement.h"



UIElement::UIElement(Render* render, Mesh* mesh, Material* material)
{
	drawnMesh = DrawnMesh(render, mesh, material);
	color = DirectX::XMFLOAT3(1, 1, 1);

	/*ShaderInfoElement<float> aspectData;
	aspectData.shaderIndex = 5;
	aspectData.data = 1;
	drawnMesh.GetIndividualVertexInfo()->AddFloat(aspectData);*/
	ShaderInfoElement<XMFLOAT3> scaleData;
	scaleData.shaderIndex = 3;
	scaleData.data = DirectX::XMFLOAT3(1, 1, 1);
//	drawnMesh.GetIndividualVertexInfo()->AddFloat3(scaleData);
	ShaderInfoElement<XMFLOAT3> colorData;
	scaleData.shaderIndex = 4;
	scaleData.data = color;
	//drawnMesh.GetIndividualVertexInfo()->AddFloat3(colorData);
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
	drawnMesh.GetMaterial()->GetVertexMaterialInfo()->GetFloat(0)->data = aspectRatio;
}
