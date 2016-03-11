#pragma once
#include "Mesh.h"
#include "Material.h"

struct TextureResource {
	std::string name;
	ID3D11ShaderResourceView* srv;
};

class Resources
{
public:
	const static int MAX_NUM_MESHES = 50;//Max number of meshes we can load
	const static int MAX_NUM_TEXTURES = 200;
	Resources(ID3D11Device* newDevice, ID3D11DeviceContext* newDeviceContext);
	~Resources();

	//Mesh Stuff
	Mesh* GetMeshIfLoaded(const char* meshName);//Returns the mesh only if it is loaded
	Mesh* GetMeshAndLoadIfNotFound(const char* meshName);
	bool IsMeshLoaded(const char* meshName);
	void LoadMesh(std::string meshName);//Load a mesh for latter
	Mesh* AddMesh(std::string meshName, Vertex* vertices, int numVerts, UINT* indices, int newNumIndices);
	int GetNextMeshIndex();
	int FindMesh(std::string meshName);

	//Texture Stuff
	TextureResource* GetTextureIfLoaded(const char* textureName);
	bool IsTextureLoaded(const char* textureName);
	TextureResource* LoadTexture(std::string textureName, std::string format);
	int FindTextureIndex(std::string textureName);
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	std::string defaultModelPath;
	Mesh* meshes[MAX_NUM_MESHES];
	std::string* meshNameToIndex;
	int numberOfMeshes;

	std::string defaultTexturePath;
	TextureResource* textures[MAX_NUM_TEXTURES];
	int numberOfTextures;
};

