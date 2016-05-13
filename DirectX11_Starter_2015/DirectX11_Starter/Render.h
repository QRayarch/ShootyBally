#pragma once
#include "Transform.h"
#include "DrawnMesh.h"
#include "SimpleShader.h"
#include "Light.h"
#include "Camera.h"
#include <d3d11.h>
#include "SpriteBatch.h"

class ScreenText;

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

struct RenderTextInfo {
	XMVECTOR screenSize;
	ID3D11DeviceContext* deviceContext;
	SpriteBatch* spriteBatch;
};

class Render
{
public:
	const static int MAX_NUM_OF_RENDERED_OBJECTS = 500;//Very likely to changed
	const static int MAX_NUM_OF_RENDERED_TEXT = 200;
	const static int MAX_NUM_OF_LIGHTS = 2;

	Render(ID3D11DeviceContext* newDeviceContext);
	~Render();

	void AddToRenderList(DrawnMesh& drawnMesh);
	void AddTextToRender(ScreenText& text);
	void UpdateAndRender(Camera& camera);

	void SetScreenSize(int nW, int nH) { screenWidth = nW; screenHeight = nH; }

	GameLight& GetLight(int index) { return lights[index]; }
	void SetLight(GameLight light, int index) { if(index >= 0 && index < MAX_NUM_OF_LIGHTS) lights[index] = light; }

private:
	ID3D11DeviceContext* deviceContext;
	DrawnMesh* renderList[MAX_NUM_OF_RENDERED_OBJECTS];
	GameLight lights[MAX_NUM_OF_LIGHTS];
	int endIndex;

	RenderInfo renderInfo;

	SpriteBatch* spriteBatch;

	ScreenText* textList[MAX_NUM_OF_RENDERED_TEXT];
	int endTextIndex;
	int screenWidth;
	int screenHeight;
};

