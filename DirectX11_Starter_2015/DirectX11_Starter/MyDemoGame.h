#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"

#include "Transform.h"
#include "Mesh.h"
#include "DrawnMesh.h"
#include "Render.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Resources.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void LoadShaders(); 
	void CreateGeometry();

	void TestLoadLevel(char* mapName);

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Camera camera;
	Resources* res;
	Render* render;
	Material* basicMaterial1;
	Material* basicMaterial2;
	ID3D11ShaderResourceView* texture1SRC;
	ID3D11ShaderResourceView* texture1NSRC;//Normal
	ID3D11ShaderResourceView* texture2SRC;
	ID3D11ShaderResourceView* texture2NSRC;//Normal
	ID3D11SamplerState* samplerState;
	EntitySystem* entSys;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	POINT curMousePos;
};