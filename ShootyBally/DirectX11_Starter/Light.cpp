#include "Light.h"

GameLight::GameLight()
{
	renderLight.AmbientColor = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
	renderLight.DiffuseColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1);
	renderLight.Type = LIGHT_DIRECTIONAL;
}

GameLight::GameLight(int type, DirectX::XMFLOAT4 newAmbientColor, DirectX::XMFLOAT4 newDiffuseColor)
{
	renderLight.AmbientColor = newAmbientColor;
	renderLight.DiffuseColor = newDiffuseColor;
	renderLight.Type = type;
}

GameLight::~GameLight()
{
}

RenderLight& GameLight::GetRenderLightData()
{
	if (renderLight.Type == LIGHT_DIRECTIONAL) {
		renderLight.Fluid3 = transform.GetForwardVector();
	}
	else if (renderLight.Type == LIGHT_POINT) {
		renderLight.Fluid3 = transform.GetPosition();
	}
	return renderLight;
}
