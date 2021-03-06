// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include "Vertex.h"

#include <time.h>
#include <fstream>
#include <iostream>
#include "Logger.h"
#include "DebugDraw.h"
#include "Input.h"

// For the DirectX Math library
using namespace DirectX;

#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(193);
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"Shooty Bally";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	/*for (int e = 0; e < ents.size(); e++) {
		delete ents[e];
	}*/

	delete render;

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;
	delete pixelShaderNoNormals;
	delete vSSkybox;
	delete pSSkybox;
	delete vsUI;
	delete psUI;
	delete spawnVS;
	delete spawnGS;
	delete particleVS;
	delete particleGS;
	delete particlePS;

	if (samplerState != nullptr) {
		samplerState->Release();
		samplerState = nullptr;
	}

	delete entSys;
	delete res;
	delete canvas;

	depthState->Release();
	rasterState->Release();

	// Clean up particle resources.
	delete[] particleEmittersAlpha;
	randomSRV->Release();
	randomTexture->Release();
	particleBlendState->Release();
	particleDepthState->Release();

	// Post Processing
	delete postManager;
	

	DebugDraw::Release();
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.

	render = new Render(deviceContext);
	render->SetScreenSize(windowWidth, windowHeight);
	res = new Resources(device, deviceContext);
	entSys = new EntitySystem(1000);
	postManager = new PostManager(device, deviceContext);

	GameLight light1 = GameLight(LIGHT_DIRECTIONAL, XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f), XMFLOAT4(0.9f, 0.4f, 0.4f, 1.0f));
	light1.GetTransform().SetRotation(XMFLOAT3(1, 0, 0));
	GameLight light2 = GameLight(LIGHT_POINT, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.4f, 1.0f, 0.4f, 1.0f));
	//light2.in
	//light2.GetTransform().SetRotation(XMFLOAT3(1, -1, 0));
	render->SetLight(light1, 0);
	render->SetLight(light2, 1);

	canvas = new Canvas(entSys, render, res);

	LoadShaders(); 
	CreateGeometry();
	TestLoadLevel("Assets/Maps/Untitled.txt");

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	camera = Camera(0.0f, 0.0f, -5.0f);
	camera.CreatePerspectiveProjectionMatrix(aspectRatio, 0.1f, 100.0f);
	DebugDraw::SetUp(device, deviceContext, &camera);

	gameState = ready;

	// Successfully initialized
	return true;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	vSSkybox = new SimpleVertexShader(device, deviceContext);
	vSSkybox->LoadShaderFile(L"VS_Skybox.cso");
	pSSkybox = new SimplePixelShader(device, deviceContext);
	pSSkybox->LoadShaderFile(L"PS_Skybox.cso");

	vsUI = new SimpleVertexShader(device, deviceContext);
	vsUI->LoadShaderFile(L"VS_UI.cso");
	psUI = new SimplePixelShader(device, deviceContext);
	psUI->LoadShaderFile(L"PS_UI.cso");

	// Load particle shaders
	spawnVS = new SimpleVertexShader(device, deviceContext);
	spawnVS->LoadShaderFile(L"VS_ParticleSpawn.cso");

	spawnGS = new SimpleGeometryShader(device, deviceContext, true, false);
	spawnGS->LoadShaderFile(L"GS_ParticleSpawn.cso");

	particleVS = new SimpleVertexShader(device, deviceContext);
	particleVS->LoadShaderFile(L"VS_Particle.cso");

	particleGS = new SimpleGeometryShader(device, deviceContext);
	particleGS->LoadShaderFile(L"GS_Particle.cso");

	particlePS = new SimplePixelShader(device, deviceContext);
	particlePS->LoadShaderFile(L"PS_Particle.cso");

	//This is tempory, 
	//TODO: create a common shader file. 
	pixelShaderNoNormals = new SimplePixelShader(device, deviceContext);
	pixelShaderNoNormals->LoadShaderFile(L"PixelShaderNoNormals.cso");

	//Sampler State
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	// Create the sky rasterizer state
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rastDesc, &rasterState);

	// Sky's depth state
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&depthDesc, &depthState);

	// Particle states
	// Blend state
	D3D11_BLEND_DESC particleBlendDesc;
	ZeroMemory(&particleBlendDesc, sizeof(D3D11_BLEND_DESC));
	particleBlendDesc.AlphaToCoverageEnable = false;
	particleBlendDesc.IndependentBlendEnable = false;
	particleBlendDesc.RenderTarget[0].BlendEnable = true;
	particleBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//particleBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;   // Additive blending.
	particleBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;   // Alpha blending (transparency).
	particleBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;   // Additive blending.
	//particleBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;   // Alpha blending (transparency).
	particleBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	particleBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	particleBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	particleBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&particleBlendDesc, &particleBlendState);

	// Depth state
	D3D11_DEPTH_STENCIL_DESC particleDepthDesc;
	ZeroMemory(&particleDepthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	particleDepthDesc.DepthEnable = true;
	particleDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	particleDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	device->CreateDepthStencilState(&particleDepthDesc, &particleDepthState);

	///////////////////////////////////////////////
	//Post Processing
	///////////////////////////////////////////////
	postManager->LoadShaders();
	postManager->BuildResources(windowWidth, windowHeight);
	postManager->SetChainDest(renderTargetView);

	
}

//This is here temporarily till more things are figured out
void MyDemoGame::TestLoadLevel(char* mapName) {
	std::ifstream s(mapName);
	if (!s.is_open()) {
		return;
	}

	int state = 0;
	//0 is the meta data
	//1 is the turrets
	//2 is light
	//3 is background
	//4 is arena

	//Stuff to keep track of info
	int lightIndex = -1;

	char chars[400];
	Transform* currentTransform = nullptr;
	Entity* currentEntity = nullptr;
	while (s.good())
	{
		s.getline(chars, 400);
		std::string line(chars);
		bool statelessRead = false;
		//LogText(chars);
		if (std::strstr(chars, "arena")) {
			statelessRead = true;
			state = 4;
		}
		else if (std::strstr(chars, "lights"))
		{
			statelessRead = true;
			state = 2;
		}
		else if (std::strstr(chars, "pos")) {
			DirectX::XMFLOAT3 pos;
			sscanf_s(chars, "pos %f %f %f", &pos.x, &pos.y, &pos.z);
			if (currentTransform != nullptr) {
				currentTransform->SetPosition(pos);
			}
			statelessRead = true;
		}
		else if (std::strstr(chars, "rot")) {
			DirectX::XMFLOAT3 rot;
			sscanf_s(chars, "rot %f %f %f", &rot.x, &rot.y, &rot.z);
			if (currentTransform != nullptr) {
				currentTransform->SetRotation(rot);
			}
			statelessRead = true;
		}
		else if (std::strstr(chars, "scl")) {
			DirectX::XMFLOAT3 scl;
			sscanf_s(chars, "scl %f %f %f", &scl.x, &scl.y, &scl.z);
			if (currentTransform != nullptr) {
				currentTransform->SetScale(scl);
			}
			statelessRead = true;
		}
		else if (std::strstr(chars, "texture")) {
			if (currentEntity != nullptr) {
				char texture1Name[31] = "";
				char texture2Name[31] = "";
				char texture3Name[31] = "";
				sscanf_s(chars, "texture %s %s %s", texture1Name, 30, texture2Name, 30, texture3Name, 30);

				Material* currentMaterial = nullptr;
				/*LogText(texture1Name);
				LogText(texture2Name);
				LogText(texture3Name);*/
				currentMaterial = res->CreateMaterial(vertexShader, pixelShaderNoNormals, samplerState, texture1Name, texture2Name, texture3Name);

				DrawnMesh* drawnMesh = currentEntity->GetComponent<DrawnMesh>();
				if (drawnMesh != nullptr) {
					if (currentMaterial != nullptr) {
						drawnMesh->SetMaterial(currentMaterial);
					}
				}
			}
		}
		if (!statelessRead) {
			switch (state)
			{
			case 2://Light loading
				if (std::strstr(chars, "lType"))
				{
					lightIndex += 1;
					currentTransform = &render->GetLight(lightIndex).GetTransform();
					int type = LIGHT_DIRECTIONAL;
					sscanf_s(chars, "lType %d", &type);
					render->GetLight(lightIndex).SetType(type);
				}
				else if (std::strstr(chars, "color")) {
					DirectX::XMFLOAT4 color;
					sscanf_s(chars, "color %f %f %f %f", &color.x, &color.y, &color.z, &color.w);
					render->GetLight(lightIndex).SetColor(color);
				}
				else if (std::strstr(chars, "lIntensity")) {
					float intensity = 1.0f;
					sscanf_s(chars, "lIntensity %f", &intensity);
					render->GetLight(lightIndex).SetIntensity(intensity);
				}
				break;
			case 4://Arena loading
				if (std::strstr(chars, "model")) 
				{
					std::string modelName = line.substr(6, line.length());
					Mesh* newMesh = nullptr;
					if (!res->IsMeshLoaded(modelName.c_str())) {
						res->LoadMesh(modelName.c_str());
					}
					newMesh = res->GetMeshIfLoaded(modelName.c_str());
					if (newMesh != nullptr) {
						currentEntity = entSys->AddEntity();
						if (currentEntity != nullptr) {
							currentTransform = &currentEntity->GetTransform();
							currentEntity->AddComponent(new DrawnMesh(render, newMesh, nullptr));
							if (modelName == "TestWall") {
								currentEntity->AddComponent(new CollisionBox(newMesh->GetVertices(), newMesh->GetNumberOfVertices()));
								walls[wallsIndex] = currentEntity;
								wallsIndex++;
							}
							else if (modelName == "TestWedge") {
								currentEntity->AddComponent(new CollisionBox(newMesh->GetVertices(), newMesh->GetNumberOfVertices()));
								wallCorners[wallCornersIndex] = currentEntity;
								wallCornersIndex++;
							}
						}
						else {
							currentTransform = nullptr;
						}
					}
					else {
						currentEntity = nullptr;
						currentTransform = nullptr;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	skyboxMesh = res->GetMeshAndLoadIfNotFound("cube");
	skyTexture = res->LoadTexture("SunnyCubeMap", Resources::FILE_FORMAT_DDS);



	Material* material1 = res->CreateMaterial(vertexShader, pixelShader, samplerState, "BrickOldMixedSize", "Normal_BrickOldMixedSize");
	Material* material2 = res->CreateMaterial(vertexShader, pixelShader, samplerState, "RockSmooth", "Normal_RockSmooth");
	Material* material3 = res->CreateMaterial(vertexShader, pixelShaderNoNormals, samplerState, "Ball");
	Material* paddleMat = res->CreateMaterial(vertexShader, pixelShaderNoNormals, samplerState, "paddle");
	Material* bulletMat = res->CreateMaterial(vertexShader, pixelShaderNoNormals, samplerState, "Bullet");
	Material* greenMat = res->CreateMaterial(vertexShader, pixelShaderNoNormals, samplerState, "green");
	Material* uiMat = res->CreateMaterial(vsUI, psUI, samplerState, "UI_Panel");//WoodRough

	// Particle emitter resources.
	particleTexture = res->LoadTexture("particle", Resources::FILE_FORMAT_PNG);
	particleEmittersAlphaLength = 64;
	particleEmittersAlpha = new ParticleEmitter*[particleEmittersAlphaLength];
	int particleEmittersAlphaIndex = 0;
	goalParticleEmitterDelay = 2.0f;
	goalParticleEmitterTimer = -1.0f;

	for (int i = 0; i < particleEmittersAlphaLength; ++i)
	{
		particleEmittersAlpha[i] = new ParticleEmitter();
		Entity* particleEntity = entSys->AddEntity();
		particleEntity->AddComponent(particleEmittersAlpha[i]);
	}

	//Ball
	Mesh* mesh1 = res->GetMeshAndLoadIfNotFound("Ball");
	Entity* ball = entSys->AddEntity();
	Transform& transform1 = ball->GetTransform();
	ball->AddComponent(new DrawnMesh(render, mesh1, material3));
	ball->AddComponent(new PhysicsBody(&transform1, 0.35f));
	ball->AddComponent(new CollisionCircle(mesh1->GetVertices(), mesh1->GetNumberOfVertices()));
	ball->GetComponent<PhysicsBody>()->SetVelocity(XMFLOAT4(0.0f, 0.0f, -0.0f, 0.0f));
	ball->AddComponent(new VelocityRotator(2.5f));
	transform1.SetPosition(XMFLOAT3(0.0f, -8.0f, 0.0f));
	render->GetLight(1).GetTransform().SetParent(&transform1);

	ballCollider = ball->GetComponent<CollisionCircle>();
	ballPhysicsBody = ball->GetComponent<PhysicsBody>();
	srand(time(NULL));
	ballPhysicsBody->SetVelocity(XMFLOAT4(0.0f, 0.0f, rand() % 6 - 3, 0.0f));

	//Generates a rectangle
	/*float halfSize = 1.0f;
	XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
	XMFLOAT3 tangent = XMFLOAT3(0, 0, 0);
	Vertex* vertices2 = new Vertex[4];
	vertices2[0] = { XMFLOAT3(-halfSize, +halfSize, 0), normal, XMFLOAT2(0, 1), tangent };
	vertices2[1] = { XMFLOAT3(+halfSize, -halfSize, 0), normal, XMFLOAT2(1, 0), tangent };
	vertices2[2] = { XMFLOAT3(-halfSize, -halfSize, 0), normal, XMFLOAT2(0, 0), tangent };
	vertices2[3] = { XMFLOAT3(+halfSize, +halfSize, 0), normal, XMFLOAT2(1, 1), tangent };
	UINT indices2[] = { 0, 1, 2, 0, 3, 1 };
	Mesh* mesh2 = res->AddMesh("ground", vertices2, 4, indices2, 6);*/

	//Generates a 9 patch
	/*float halfSize = 1.0f;
	float boarderSize = 0.016f;
	XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
	XMFLOAT3 innerVertex = XMFLOAT3(1, 1, 1);
	XMFLOAT3 tangent = XMFLOAT3(0, 0, 0);
	Vertex* vertices2 = new Vertex[16];
	//Top Row
	vertices2[0] = { XMFLOAT3(-halfSize, halfSize, 0), normal, XMFLOAT2(0, 1), tangent };
	vertices2[1] = { XMFLOAT3(-halfSize + boarderSize, halfSize, 0), normal, XMFLOAT2(boarderSize, 1), tangent };
	vertices2[2] = { XMFLOAT3(+halfSize - boarderSize, halfSize, 0), normal, XMFLOAT2(1 - boarderSize, 1), tangent };
	vertices2[3] = { XMFLOAT3(halfSize, halfSize, 0), normal, XMFLOAT2(1, 1), tangent };

	//Top Lower Row
	vertices2[4] = { XMFLOAT3(-halfSize, halfSize - boarderSize, 0), normal, XMFLOAT2(0, 1 - boarderSize), tangent };
	vertices2[5] = { XMFLOAT3(-halfSize + boarderSize, halfSize - boarderSize, 0), innerVertex, XMFLOAT2(boarderSize, 1 - boarderSize), tangent };
	vertices2[6] = { XMFLOAT3(+halfSize - boarderSize, halfSize - boarderSize, 0), innerVertex, XMFLOAT2(1 - boarderSize, 1 - boarderSize), tangent };
	vertices2[7] = { XMFLOAT3(halfSize, halfSize - boarderSize, 0), normal, XMFLOAT2(1, 1 - boarderSize), tangent };

	//Bottom Upper Row
	vertices2[8] = { XMFLOAT3(-halfSize, -halfSize + boarderSize, 0), normal, XMFLOAT2(0, boarderSize), tangent };
	vertices2[9] = { XMFLOAT3(-halfSize + boarderSize, -halfSize + boarderSize, 0), innerVertex, XMFLOAT2(boarderSize, boarderSize), tangent };
	vertices2[10] = { XMFLOAT3(+halfSize - boarderSize, -halfSize + boarderSize, 0), innerVertex, XMFLOAT2(1 - boarderSize, boarderSize), tangent };
	vertices2[11] = { XMFLOAT3(halfSize, -halfSize + boarderSize, 0), normal, XMFLOAT2(1, boarderSize), tangent };

	//Bottom Row
	vertices2[12] = { XMFLOAT3(-halfSize, -halfSize, 0), normal, XMFLOAT2(0, 0), tangent };
	vertices2[13] = { XMFLOAT3(-halfSize + boarderSize, -halfSize, 0), normal, XMFLOAT2(boarderSize, 0), tangent };
	vertices2[14] = { XMFLOAT3(+halfSize - boarderSize, -halfSize, 0), normal, XMFLOAT2(1 - boarderSize, 0), tangent };
	vertices2[15] = { XMFLOAT3(halfSize, -halfSize, 0), normal, XMFLOAT2(1, 0), tangent };
	UINT indices2[] = { 0, 1, 4, 1, 5, 4, 1, 2, 5, 2, 6, 5, 2, 3, 6, 3, 7, 6,	
		4, 5, 8, 5, 9, 8, 5, 6, 9, 6, 10, 9, 6, 7, 10, 7, 11, 10,	
		8, 9, 12, 9, 13, 12, 9, 10, 13, 10, 14, 13, 10, 11, 14, 11, 15, 14};
	Mesh* mesh2 = res->AddMesh("ground", vertices2, 16, indices2, 54);

	Entity* entity2 = entSys->AddEntity();

	ShaderInfoElement<float> aspectData;
	aspectData.shaderIndex = 5;
	aspectData.data = aspectRatio;
	uiMat->GetVertexMaterialInfo()->AddFloat(aspectData);
	entity2->AddComponent(new DrawnMesh(render, mesh2, uiMat));
	entity2->GetTransform().SetScale(XMFLOAT3(0.5f, 0.5f, 0.0f));
	ShaderInfoElement<XMFLOAT3> scaleData;
	scaleData.shaderIndex = 3;
	scaleData.data = entity2->GetTransform().GetScale();
	uiMat->GetVertexMaterialInfo()->AddFloat3(scaleData);
	ShaderInfoElement<XMFLOAT3> colorData;
	colorData.shaderIndex = 4;
	colorData.data = XMFLOAT3(1.0f, 1.0f, 1.0f);
	uiMat->GetVertexMaterialInfo()->AddFloat3(colorData);
	//entity2->GetTransform().SetPosition(XMFLOAT3(-1 + 0.25f, 1 - 0.25f, 0));*/

	//Players
	Mesh* mesh3 = res->GetMeshAndLoadIfNotFound("sbgPaddle");
	float paddleScale = 0.5f;
	goalPlayer = nullptr;
	
	Entity* entity3 = entSys->AddEntity();
	entity3->AddComponent(new CollisionCircle(mesh3->GetVertices(), mesh3->GetNumberOfVertices()));
	entity3->AddComponent(new CollisionBox(mesh3->GetVertices(), mesh3->GetNumberOfVertices()));
	entity3->AddComponent(new DrawnMesh(render, mesh3, paddleMat));
	Transform& transform3 = entity3->GetTransform();
	entity3->AddComponent(new PhysicsBody(&transform3, 50000.0f));
	transform3.SetPosition(XMFLOAT3(-5.75f, -7.5f, 0.0f));
	transform3.SetRotation(XMFLOAT3(0.0f, XM_PI / 2, 0));
	transform3.SetScale(XMFLOAT3(paddleScale, paddleScale, paddleScale));

	Entity* entity4 = entSys->AddEntity();
	entity4->AddComponent(new CollisionCircle(mesh3->GetVertices(), mesh3->GetNumberOfVertices()));
	entity4->AddComponent(new CollisionBox(mesh3->GetVertices(), mesh3->GetNumberOfVertices()));
	entity4->AddComponent(new DrawnMesh(render, mesh3, paddleMat));
	Transform& transform4 = entity4->GetTransform();
	entity4->AddComponent(new PhysicsBody(&transform4, 50000.0f));
	transform4.SetPosition(XMFLOAT3(5.75f, -7.5f, 0.0f));
	transform4.SetRotation(XMFLOAT3(0.0f, -XM_PI / 2, 0.0f));
	transform4.SetScale(XMFLOAT3(paddleScale, paddleScale, paddleScale));

	// Initialize player 1 goal particle emitters.
	particleEmittersAlpha[particleEmittersAlphaIndex]->Init(
		&transform4,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 100.0f, 0.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		30.0f,
		30.0f,
		30.0f,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		0,
		0.00001f,
		2.0f,
		2.0f,
		particleTexture,
		device);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferReadPointer(), 1000000);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferWritePointer(), 1000000);
	++particleEmittersAlphaIndex;

	particleEmittersAlpha[particleEmittersAlphaIndex]->Init(
		&transform4,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 10.0f, 0.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 0.5f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 0.1f),
		20.0f,
		10.0f,
		0.2f,
		XMFLOAT3(0.0f, -10.0f, 0.0f),
		30,
		10000.0f,
		2.0f,
		2.0f,
		particleTexture,
		device);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferReadPointer(), 1000000);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferWritePointer(), 1000000);
	player1 = Player(entity3, 1, particleEmittersAlpha[particleEmittersAlphaIndex - 1], particleEmittersAlpha[particleEmittersAlphaIndex], bulletPool);
	++particleEmittersAlphaIndex;

	// Initialize player 2 goal particle emitters.
	particleEmittersAlpha[particleEmittersAlphaIndex]->Init(
		&transform3,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 100.0f, 0.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		30.0f,
		30.0f,
		30.0f,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		0,
		0.00001f,
		2.0f,
		2.0f,
		particleTexture,
		device);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferReadPointer(), 1000000);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferWritePointer(), 1000000);
	++particleEmittersAlphaIndex;

	particleEmittersAlpha[particleEmittersAlphaIndex]->Init(
		&transform3,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 10.0f, 0.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 0.5f),
		XMFLOAT4(0.1f, 0.1f, 1.0f, 0.1f),
		20.0f,
		10.0f,
		0.2f,
		XMFLOAT3(0.0f, -10.0f, 0.0f),
		30,
		10000.0f,
		2.0f,
		2.0f,
		particleTexture,
		device);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferReadPointer(), 1000000);
	spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferWritePointer(), 1000000);
	player2 = Player(entity4, 2, particleEmittersAlpha[particleEmittersAlphaIndex - 1], particleEmittersAlpha[particleEmittersAlphaIndex], bulletPool);
	++particleEmittersAlphaIndex;

	Mesh* torusMesh = res->GetMeshAndLoadIfNotFound("torus");
	Entity* torus1 = entSys->AddEntity();
	torus1->AddComponent(new DrawnMesh(render, torusMesh, greenMat));
	torus1->GetTransform().SetPosition(XMFLOAT3(transform3.GetPosition()));
	torus1->GetTransform().SetScale(XMFLOAT3(1.75f, 1.75f, 1.75f));
	Entity* torus2 = entSys->AddEntity();
	torus2->AddComponent(new DrawnMesh(render, torusMesh, greenMat));
	torus2->GetTransform().SetPosition(XMFLOAT3(transform4.GetPosition()));
	torus2->GetTransform().SetScale(XMFLOAT3(1.75f, 1.75f, 1.75f));

	//Bullets
	Mesh* mesh4 = res->GetMeshAndLoadIfNotFound("hpBullet");
	for (int i = 0; i < poolSize; i++)
	{
		Entity* bulletEntity = entSys->AddEntity();
		Transform& bulletTransform = bulletEntity->GetTransform();
		bulletEntity->AddComponent(new DrawnMesh(render, mesh4, bulletMat));
		bulletEntity->AddComponent(new PhysicsBody(&bulletTransform, 1.0f));
		bulletEntity->AddComponent(new CollisionCircle(mesh4->GetVertices(), mesh4->GetNumberOfVertices()));
		bulletTransform.SetPosition(XMFLOAT3(0.0f, 5.0f, 0.0f));
		bulletTransform.SetScale(XMFLOAT3(0.08f, 0.08f, 0.08f));

		// Initialize bullet trail particle emitter.
		particleEmittersAlpha[particleEmittersAlphaIndex]->Init(
			&bulletTransform,
			XMFLOAT3(0.0f, 0.0f, -5.0f),
			XMFLOAT3(0.0f, 0.0f, -4.0f),
			XMFLOAT4(1.0f, 0.1f, 0.1f, 0.3f),
			XMFLOAT4(1.0f, 1.0f, 0.1f, 0.2f),
			XMFLOAT4(1.0f, 0.5f, 0.1f, 0.1f),
			2.0f,
			1.0f,
			0.2f,
			XMFLOAT3(0.0f, 0.0f, -12.0f),
			0,
			0.00001f,
			2.0f,
			2.0f,
			particleTexture,
			device);
		spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferReadPointer(), 1000000);
		spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferWritePointer(), 1000000);
		++particleEmittersAlphaIndex;

		// Initialize bullet explosion particle emitter.
		particleEmittersAlpha[particleEmittersAlphaIndex]->Init(
			&bulletTransform,
			XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT3(0.0f, 10.0f, 0.0f),
			XMFLOAT4(1.0f, 0.1f, 0.1f, 0.8f),
			XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f),
			XMFLOAT4(0.5f, 0.5f, 0.5f, 0.1f),
			10.0f,
			5.0f,
			2.0f,
			XMFLOAT3(0.0f, -20.0f, 0.0f),
			30,
			10000.0f,
			2.0f,
			2.0f,
			particleTexture,
			device);
		spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferReadPointer(), 1000000);
		spawnGS->CreateCompatibleStreamOutBuffer(particleEmittersAlpha[particleEmittersAlphaIndex]->GetSoBufferWritePointer(), 1000000);

		bulletPool[i] = Bullet(bulletEntity, particleEmittersAlpha[particleEmittersAlphaIndex - 1], particleEmittersAlpha[particleEmittersAlphaIndex]);

		++particleEmittersAlphaIndex;
	}

	//UI
	canvas->GetHoverButtonState().color = XMFLOAT3(0, 1, 1);
	canvas->GetHoverButtonState().scale = 1.08f;
	canvas->GetHoverButtonState().transitionTime = 0.1f;
	canvas->GetDefualtButtonState().transitionTime = 0.1f;
	//Button* button = canvas->AddButton({ 0.89f, 0.90f, 0.1f, 0.08f }, uiMat, L"Reset", XMFLOAT4(0.2f, 0.2f, 0.2f, 1));
	//button->SetOnClicked(foo);
	//canvas->AddButton({ -0.25f, -0.5f, 0.1f, 0.08f }, uiMat, L"BUTTON!", XMFLOAT4(0.2f, 0.2f, 0.2f, 1));
	float scoreOut = 0.1f;
	float scoreUp = 0.8f;
	float panelW = 0.04f;
	float panelH = 0.06f;
	canvas->AddPanel({ -scoreOut, scoreUp, panelW, panelH }, uiMat, XMFLOAT3(0.7f, 0.4f, 0.4f));
	canvas->AddPanel({ scoreOut, scoreUp, panelW, panelH }, uiMat, XMFLOAT3(0.4f, 0.4f, 0.7f));
	playerOneScore = canvas->AddText(L"0", -scoreOut, scoreUp, XMFLOAT4(1, 0.9f, 0.9f, 1));
	playerOneScore->CenterText();
	playerTwoScore = canvas->AddText(L"0", scoreOut, scoreUp, XMFLOAT4(0.9f, 0.9f, 1, 1));
	playerTwoScore->CenterText();

	//END UI

	// Set up "random" resources.
	unsigned int randomTextureWidth = 1024;

	// Random data for the 1D texture.
	srand((unsigned int)time(0));
	std::vector<float> data(randomTextureWidth * 4);
	for (unsigned int i = 0; i < randomTextureWidth * 4; i++)
		data[i] = rand() / (float)RAND_MAX * 2.0f - 1.0f;

	// Set up the random texture.
	D3D11_TEXTURE1D_DESC textureDesc;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Width = 100;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = (void*)&data[0];
	initData.SysMemPitch = randomTextureWidth * sizeof(float) * 4;
	initData.SysMemSlicePitch = 0;
	device->CreateTexture1D(&textureDesc, &initData, &randomTexture);

	// Set up the SRV for the random texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	srvDesc.Texture1D.MipLevels = 1;
	srvDesc.Texture1D.MostDetailedMip = 0;
	device->CreateShaderResourceView(randomTexture, &srvDesc, &randomSRV);
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	// Update our projection matrix since the window size changed
	camera.CreatePerspectiveProjectionMatrix(aspectRatio, 0.1f, 100.0f);

	//Post
	if (postManager) {
		postManager->ReleaseResources();
		postManager->BuildResources(windowWidth, windowHeight);
		postManager->SetChainDest(renderTargetView);
	}
	
	/*ReleaseMacro(postRTV);
	ReleaseMacro(postSRV);*/
	//Target Texture
	//D3D11_TEXTURE2D_DESC texDesc = {};
	//texDesc.Width = windowWidth;
	//texDesc.Height = windowHeight;
	//texDesc.ArraySize = 1;
	//texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//texDesc.CPUAccessFlags = 0;
	//texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//texDesc.MipLevels = 1;
	//texDesc.MiscFlags = 0;
	//texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
	//texDesc.Usage = D3D11_USAGE_DEFAULT;
	//ID3D11Texture2D* postTexture;
	//device->CreateTexture2D(&texDesc, 0, &postTexture);

	////Render Target View
	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	//rtvDesc.Format = texDesc.Format;
	//rtvDesc.Texture2D.MipSlice = 0;
	//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//device->CreateRenderTargetView(postTexture, &rtvDesc, &postRTV);

	////Shader Resource View
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = texDesc.Format;
	//srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//device->CreateShaderResourceView(postTexture, &srvDesc, &postSRV);

	if (render != nullptr) {
		render->SetScreenSize(windowWidth, windowHeight);
	}
	if (canvas != nullptr) {
		Material* uiMat = res->GetMaterial("UI_Panel");
		canvas->SetAspectRatio(aspectRatio);
	}

}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
float x = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	Input::Update();//This should be near the begining of the frame to get a better user experence

	if (Input::IsKeyDownThisFrame(VK_OEM_3)) {
		drawDebug = !drawDebug;
	}

	DebugDraw::AddLine(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 0, 0), XMFLOAT4(1, 0, 0, 1));
	DebugDraw::AddLine(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1, 0), XMFLOAT4(0, 1, 0, 1));
	DebugDraw::AddLine(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1), XMFLOAT4(0, 0, 1, 1));

	DebugDraw::AddSphere(XMFLOAT3(0, 0, 0), 1, XMFLOAT4(1, 1, 1, 1));

	//Paddle Collisions
	if (ballCollider->IsColliding(player1.GetCircleCollider())) {
		if (player1.GetBoxCollider()->IsColliding(ballCollider)) {
			player1.GetPlayerPB()->ResolveCollisions(ballPhysicsBody);
		}
		else {
			GoalScored(player2);
		}
	}
	if (ballCollider->IsColliding(player2.GetCircleCollider())) {
		if (player2.GetBoxCollider()->IsColliding(ballCollider)) {
			player2.GetPlayerPB()->ResolveCollisions(ballPhysicsBody);
		}
		else {
			GoalScored(player1);
		}
	}

	//Wall Collisions
	bool hasCollided = false;
	for (int i = 0; i < wallsIndex; i++) {
		if (walls[i]->GetComponent<CollisionBox>()->IsColliding(ballCollider) && !hasCollided) {
			XMFLOAT4 tempVel = ballPhysicsBody->GetVelocity();
			XMFLOAT3 tempPos = ballPhysicsBody->GetTransform().GetPosition();
			if (tempPos.x > 7.5) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(-abs(tempVel.x), 0.0f, tempVel.z, 0.0f));
			}
			else if (tempPos.x < -7.5) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(abs(tempVel.x), 0.0f, tempVel.z, 0.0f));
			}
			else if (tempPos.z > 3.8) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(tempVel.x, 0.0f, -abs(tempVel.z), 0.0f));
			}
			else if (tempPos.z < -3.8) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(tempVel.x, 0.0f, abs(tempVel.z), 0.0f));
			}
			hasCollided = true;
		}
	}

	//Corner Collisions
	for (int i = 0; i < wallCornersIndex; i++) {
		if (wallCorners[i]->GetComponent<CollisionBox>()->IsColliding(ballCollider) && !hasCollided) {
			XMFLOAT4 tempVel = ballPhysicsBody->GetVelocity();
			XMFLOAT3 tempPos = ballPhysicsBody->GetTransform().GetPosition();
			if (tempPos.x > 7.5) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(-abs(tempVel.x), 0.0f, tempVel.z, 0.0f));
			}
			else if (tempPos.x < -7.5) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(abs(tempVel.x), 0.0f, tempVel.z, 0.0f));
			}
			if (tempPos.z > 3.8) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(tempVel.x, 0.0f, -abs(tempVel.z), 0.0f));
			}
			else if (tempPos.z < -3.8) {
				ballPhysicsBody->SetVelocity(XMFLOAT4(tempVel.x, 0.0f, abs(tempVel.z), 0.0f));
			}
			hasCollided = true;
		}
	}

	//Bullet Physics & Collision Loop
	for (int i = 0; i < poolSize; i++) {
		if (bulletPool[i].GetIsActive()) {
			if (bulletPool[i].GetEntity()->GetComponent<CollisionCircle>()->IsColliding(ballCollider)) {
				bulletPool[i].GetEntity()->GetComponent<PhysicsBody>()->ResolveCollisions(ballPhysicsBody);
				bulletPool[i].SetIsActive(false);
			}
			else {
				bulletPool[i].UpdatePhysics(deltaTime);
			}
		}
	}
	ballPhysicsBody->PhysicsUpdate(deltaTime);

	
	//Player Input
	player1.GetInput(deltaTime);
	player2.GetInput(deltaTime);

	entSys->Update(deltaTime);

	// Update particle emitters.
	if (goalParticleEmitterTimer != -1.0f)
	{
		goalParticleEmitterTimer += deltaTime;

		if (goalParticleEmitterTimer >= goalParticleEmitterDelay)
		{
			goalParticleEmitterTimer = -1.0f;
			goalPlayer->DisableGoalParticles();
			goalPlayer = nullptr;
		}
	}

	for (int i = 0; i < particleEmittersAlphaLength; ++i)
	{
		particleEmittersAlpha[i]->ParticlesUpdate(deltaTime);
	}

	//Temp camera and input stuff
	camera.Update(deltaTime, Input::GetMouseDeltaX(), Input::GetMouseDeltaY());

	//THIS HAS TO GO LAST FOR THE FRAME
	Input::EndFrameUpdate();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Handle spawning particles.
	DrawSpawn(deltaTime, totalTime);

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	//Swap for post processing
	ID3D11RenderTargetView** postRTV = postManager->GetChainRTVStart();
	deviceContext->OMSetRenderTargets(1, postRTV, depthStencilView);



	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(*postRTV, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	if (drawDebug) {
		DebugDraw::DrawAll(false);
	}
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	render->UpdateAndRender(camera);

	//TODO: handle skybox better
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, skyboxMesh->GetVertexBuffer(), &stride, &offset);
	deviceContext->IASetIndexBuffer(skyboxMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	vSSkybox->SetMatrix4x4(0, camera.GetViewMatrix());
	vSSkybox->SetMatrix4x4(1, camera.GetProjectionMatrix());
	vSSkybox->SetShader();

	pSSkybox->SetShaderResourceView(0, skyTexture);
	pSSkybox->SetShader();
	deviceContext->RSSetState(rasterState);
	deviceContext->OMSetDepthStencilState(depthState, 0);
	deviceContext->DrawIndexed(skyboxMesh->GetNumberOfIndices(), 0, 0);

	// Reset states
	deviceContext->RSSetState(0);
	deviceContext->OMSetDepthStencilState(0, 0);

	// Draw particles.
	for (int i = 0; i < particleEmittersAlphaLength; ++i)
	{
		if (particleEmittersAlpha[i]->GetEnabled())
		{
			particleVS->SetFloat3("acceleration", particleEmittersAlpha[i]->GetConstantAccel());
			particleVS->SetFloat("maxLifetime", particleEmittersAlpha[i]->GetMaxLifetime());

			particleGS->SetMatrix4x4("world", particleEmittersAlpha[i]->GetTransform().GetWorldMatrix());
			particleGS->SetMatrix4x4("view", camera.GetViewMatrix());
			particleGS->SetMatrix4x4("projection", camera.GetProjectionMatrix());

			particlePS->SetSamplerState("trilinear", samplerState);
			particlePS->SetShaderResourceView("particleTexture", particleEmittersAlpha[i]->GetTexture());

			particleVS->SetShader(true);
			particleGS->SetShader(true);
			particlePS->SetShader(true);

			// Set up states.
			float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			deviceContext->OMSetBlendState(particleBlendState, factor, 0xffffffff);
			deviceContext->OMSetDepthStencilState(particleDepthState, 0);

			// Set buffers.
			UINT particleStride = sizeof(ParticleVertex);
			UINT particleOffset = 0;
			deviceContext->IASetVertexBuffers(0, 1, particleEmittersAlpha[i]->GetSoBufferReadPointer(), &particleStride, &particleOffset);

			// Draw auto - draws based on current stream out buffer.
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			deviceContext->DrawAuto();

			// Unset Geometry Shader and reset states.
			deviceContext->GSSetShader(0, 0, 0);
			deviceContext->OMSetBlendState(0, factor, 0xffffffff);
			deviceContext->OMSetDepthStencilState(0, 0);
		}
	}




	/////////////////
	//Post Processing
	/////////////////
	postManager->RunChain(windowWidth, windowHeight, samplerState, stride, offset);
	
	HR(swapChain->Present(0, 0));
}

// Handles the "drawing" of particle spawns.
void MyDemoGame::DrawSpawn(float dt, float totalTime)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	for (int i = 0; i < particleEmittersAlphaLength; ++i)
	{
		if (particleEmittersAlpha[i]->GetEnabled())
		{
			float ageToSpawn;
			if (particleEmittersAlpha[i]->GetDisableTimer() == -1.0f)
			{
				ageToSpawn = particleEmittersAlpha[i]->GetAgeToSpawn();
			}
			else
			{
				// The particle emitter is in the process of disabling, so stop spawning new particles.
				ageToSpawn = 10000.0f;
			}

			int numSpawn = 0;
			int particleEmitterFrameCount = particleEmittersAlpha[i]->GetFrameCount();
			if (particleEmitterFrameCount == 0)
			{
				numSpawn = particleEmittersAlpha[i]->GetNumSpawn();
			}

			// Set/unset correct shaders.
			// Set the delta time for the spawning.
			spawnGS->SetFloat("dt", dt);
			spawnGS->SetFloat("ageToSpawn", ageToSpawn);
			spawnGS->SetFloat("maxLifetime", particleEmittersAlpha[i]->GetMaxLifetime());
			spawnGS->SetFloat("totalTime", totalTime);
			spawnGS->SetInt("numSpawn", numSpawn);
			spawnGS->SetSamplerState("randomSampler", samplerState);
			spawnGS->SetShaderResourceView("randomTexture", randomSRV);

			spawnVS->SetShader();
			spawnGS->SetShader();
			deviceContext->PSSetShader(0, 0, 0); // No pixel shader needed.

			// Unbind vertex buffers (incase).
			ID3D11Buffer* unset = 0;
			deviceContext->IASetVertexBuffers(0, 1, &unset, &stride, &offset);

			// First frame?
			if (particleEmitterFrameCount == 0)
			{
				// Draw using the seed vertex.
				deviceContext->IASetVertexBuffers(0, 1, particleEmittersAlpha[i]->GetVertexBufferPointer(), &stride, &offset);
				deviceContext->SOSetTargets(1, particleEmittersAlpha[i]->GetSoBufferWritePointer(), &offset);
				deviceContext->Draw(1, 0);
				particleEmittersAlpha[i]->SetFrameCount(particleEmitterFrameCount + 1);
			}
			else
			{
				// Draw using the buffers.
				deviceContext->IASetVertexBuffers(0, 1, particleEmittersAlpha[i]->GetSoBufferReadPointer(), &stride, &offset);
				deviceContext->SOSetTargets(1, particleEmittersAlpha[i]->GetSoBufferWritePointer(), &offset);
				deviceContext->DrawAuto();
			}

			// Unbind SO targets and shader.
			SimpleGeometryShader::UnbindStreamOutStage(deviceContext);
			deviceContext->GSSetShader(0, 0, 0);

			// Swap after draw.
			particleEmittersAlpha[i]->SwapSoBuffers();
		}
	}
}

void MyDemoGame::GoalScored(Player& player)
{
	ballCollider->GetEntity()->GetTransform().SetPosition(XMFLOAT3(0.0f, -7.5f, 0.0f));
	ballPhysicsBody->SetVelocity(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	player.AddPoint();
	if (goalPlayer != nullptr)
	{
		goalPlayer->DisableGoalParticles();
	}
	player.EnableGoalParticles();
	goalParticleEmitterTimer = 0.0f;
	goalPlayer = &player;
	if (playerOneScore != nullptr) {
		playerOneScore->SetTextAsInt(player1.GetScore());
		playerOneScore->CenterText();
	}
	if (playerTwoScore != nullptr) {
		playerTwoScore->SetTextAsInt(player2.GetScore());
		playerTwoScore->CenterText();
	}
	for (int i = 0; i < poolSize; i++)
	{
		if (bulletPool[i].GetIsActive())
		{
			bulletPool[i].SetIsActive(false);
		}
	}
}

void MyDemoGame::OnReset()
{
	player1.ResetPlayer();
	player2.ResetPlayer();
	ballCollider->GetEntity()->GetTransform().SetPosition(XMFLOAT3(0.0f, -7.5f, 0.0f));
	ballPhysicsBody->SetVelocity(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	if (playerOneScore != nullptr) {
		playerOneScore->SetTextAsInt(player1.GetScore());
		playerOneScore->CenterText();
	}
	if (playerTwoScore != nullptr) {
		playerTwoScore->SetTextAsInt(player2.GetScore());
		playerTwoScore->CenterText();
	}
	for (int i = 0; i < poolSize; i++)
	{
		if (bulletPool[i].GetIsActive())
		{
			bulletPool[i].SetIsActive(false);
		}
	}
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	Input::SetMouseInfo(btnState, x, y, windowWidth, windowHeight);

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	Input::SetMouseInfo(btnState, x, y, windowWidth, windowHeight);
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	Input::SetMouseInfo(btnState, x, y, windowWidth, windowHeight);
}
#pragma endregion