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
#include "Player.h"
#include "PhysicsBody.h"
#include "Bullet.h"
#include "CollisionBox.h"
#include "CollisionCircle.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

enum GameState { ready, playing, scored };

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

	// Particle helper functions.
	void DrawSpawn(float dt, float totalTime);
	void SwapSOBuffers();

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Camera camera;
	Resources* res;
	Render* render;
//	std::vector<Material*> materials;
	ID3D11SamplerState* samplerState;
	EntitySystem* entSys;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* pixelShaderNoNormals;
	SimplePixelShader* psUI;
	SimpleVertexShader* vsUI;

	SimpleVertexShader* vSSkybox;
	SimplePixelShader* pSSkybox;
	Mesh* skyboxMesh;
	ID3D11ShaderResourceView* skyTexture;
	ID3D11RasterizerState*		rasterState;
	ID3D11DepthStencilState*	depthState;

	// Particle resources.
	ID3D11Buffer* particleVB;
	ID3D11Buffer* soBufferRead;
	ID3D11Buffer* soBufferWrite;
	bool spawnFlip;
	int frameCount;

	SimpleVertexShader* particleVS;
	SimplePixelShader* particlePS;
	SimpleGeometryShader* particleGS;

	SimpleVertexShader* spawnVS;
	SimpleGeometryShader* spawnGS;

	ID3D11Texture1D* randomTexture;
	ID3D11ShaderResourceView* randomSRV;
	ID3D11ShaderResourceView* particleTexture;
	ID3D11BlendState* particleBlendState;
	ID3D11DepthStencilState* particleDepthState;

	// Particle params.
	DirectX::XMFLOAT3 particleStartPosition;
	DirectX::XMFLOAT3 particleStartVelocity;
	DirectX::XMFLOAT4 particleStartColor;
	DirectX::XMFLOAT4 particleMidColor;
	DirectX::XMFLOAT4 particleEndColor;
	float particleStartSize;
	float particleMidSize;
	float particleEndSize;

	float particleAgeToSpawn;
	float particleMaxLifetime;
	DirectX::XMFLOAT3 particleConstantAccel;

	Player player1;
	Player player2;

	int poolSize = 30;
	Bullet bulletPool[30];

	GameState gameState;
	bool drawDebug;
};