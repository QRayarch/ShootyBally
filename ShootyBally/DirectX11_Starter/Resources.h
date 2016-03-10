#pragma once
#include "Mesh.h"
#include "Material.h"

class Resources
{
public:
	const static int MAX_NUM_MESHES = 50;//Max number of meshes we can load
	Resources(ID3D11Device* newDevice);
	~Resources();

	Mesh* GetMeshIfLoaded(const char* meshName);//Returns the mesh only if it is loaded
	Mesh* GetMeshAndLoadIfNotFound(const char* meshName);
	bool IsMeshLoaded(const char* meshName);
	void LoadMesh(std::string meshName);//Load a mesh for latter
	Mesh* AddMesh(std::string meshName, Vertex* vertices, int numVerts, UINT* indices, int newNumIndices);
	int GetNextMeshIndex();
	int FindMesh(std::string meshName);
private:
	std::string defaultModelPath;
	ID3D11Device* device;
	Mesh* meshes[MAX_NUM_MESHES];
	std::string* meshNameToIndex;
	int numberOfMeshes;
};

