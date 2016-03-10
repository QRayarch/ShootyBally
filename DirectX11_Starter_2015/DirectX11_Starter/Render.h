#pragma once
#include "Transform.h"
#include "DrawnMesh.h"
#include "SimpleShader.h"
#include "Light.h"
#include "Camera.h"
#include <d3d11.h>

struct RenderInfo {
	//Critical stuff
	ID3D11DeviceContext* deviceContext;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	//Various stuff
	DirectX::XMFLOAT3 cameraPosition;

	//Light stuff
	RenderLight light1;
	RenderLight light2;

	Material* currentMaterial;
	Mesh* currentMesh;
};

class Render
{
public:
	const static int MAX_NUM_OF_RENDERED_OBJECTS = 100;//Very likely to changed
	const static int MAX_NUM_OF_LIGHTS = 2;

	Render(ID3D11DeviceContext* newDeviceContext);
	~Render();

	void AddToRenderList(DrawnMesh& drawnMesh);
	void UpdateAndRender(Camera& camera);

	GameLight& GetLight(int index) { return lights[index]; }
	void SetLight(GameLight light, int index) { lights[index] = light; }

private:
	ID3D11DeviceContext* deviceContext;
	DrawnMesh* renderList[MAX_NUM_OF_RENDERED_OBJECTS];
	GameLight lights[MAX_NUM_OF_LIGHTS];
	int endIndex;

	RenderInfo renderInfo;
};

