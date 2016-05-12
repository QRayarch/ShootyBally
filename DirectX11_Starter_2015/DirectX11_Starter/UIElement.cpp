#include "UIElement.h"


UIElement::UIElement(Render* render, Mesh* mesh, Material* material)
	: DrawnMesh(render, mesh, material)
{
	SetColor(DirectX::XMFLOAT3(1, 1, 1));
}


UIElement::~UIElement()
{

}

void UIElement::SetColor(DirectX::XMFLOAT3 newColor)
{
	color = newColor;
	ShaderInfoElement<XMFLOAT3>* colorInfo = GetIndividualVertexInfo().GetFloat3ByShaderIndex(COLOR_SHADER_INDEX);
	if (colorInfo != nullptr) {
		colorInfo->data = color;
	}
	else {
		ShaderInfoElement<XMFLOAT3> colorData;
		colorData.shaderIndex = COLOR_SHADER_INDEX;
		colorData.data = DirectX::XMFLOAT3(1, 1, 1);
		GetIndividualVertexInfo().AddFloat3(colorData);
	}
	//GetIndividualVertexInfo().GetFloat3(1).data = color;
}

void UIElement::SetAspectRatio(float aspectRatio)
{
	ShaderInfoElement<float>* aspectInfo = GetMaterial()->GetVertexMaterialInfo().GetFloatByShaderIndex(ASPECT_RATIO_SHADER_INDEX);
	if (aspectInfo != nullptr) {
		aspectInfo->data = aspectRatio;
	}
	else 
	{
		ShaderInfoElement<float> aspectData;
		aspectData.shaderIndex = ASPECT_RATIO_SHADER_INDEX;
		aspectData.data = aspectRatio;
		GetMaterial()->GetVertexMaterialInfo().AddFloat(aspectData);
	}
}

void UIElement::SetRect(UIRect rect)
{
	GetTransform().SetPosition(DirectX::XMFLOAT3(rect.x, rect.y, 0));
	SetScale(rect.w, rect.h);
}

void UIElement::SetScale(float x, float y)
{
	DirectX::XMFLOAT3 newScale = DirectX::XMFLOAT3(x, y, 1);
	GetTransform().SetScale(newScale);//Set the proper position

	ShaderInfoElement<XMFLOAT3>* scaleInfo = GetIndividualVertexInfo().GetFloat3ByShaderIndex(SCALE_SHADER_INDEX);
	if (scaleInfo != nullptr) {
		scaleInfo->data = newScale;
	}
	else {
		ShaderInfoElement<XMFLOAT3> scaleData;
		scaleData.shaderIndex = SCALE_SHADER_INDEX;
		scaleData.data = newScale;
		GetIndividualVertexInfo().AddFloat3(scaleData);
	}
}
