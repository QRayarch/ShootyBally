#include "Resources.h"
#include <vector>
#include <fstream>
#include "Logger.h"

Resources::Resources(ID3D11Device* newDevice)
{
	defaultModelPath = "Assets/Models/";
	numberOfMeshes = 0;
	device = newDevice;
	meshNameToIndex = new std::string[MAX_NUM_MESHES];
}


Resources::~Resources()
{
	for (int m = 0; m < numberOfMeshes; m++) {
		if (meshes[m] != nullptr) {
			delete meshes[m];
			meshes[m] = nullptr;
		}
	}
	delete[] meshNameToIndex;
}

Mesh* Resources::GetMeshIfLoaded(const char * meshName)
{
	int index = FindMesh(meshName);
	if (index != -1) {
		return meshes[index];
	}
	return nullptr;
}

Mesh * Resources::GetMeshAndLoadIfNotFound(const char * meshName)
{
	int index = FindMesh(meshName);
	if (index != -1) {
		return meshes[index];
	}
	LoadMesh(meshName);
	return GetMeshIfLoaded(meshName);
}

bool Resources::IsMeshLoaded(const char * meshName)
{
	return FindMesh(meshName) != -1;
}

void Resources::LoadMesh(std::string meshName)
{
	int index = FindMesh(meshName);
	if (index != -1) {
		LogText("--Not loading Model--//Trying to load a model with a duplicate name, model will not be loaded.");
		return;
	}
	std::string filePath = defaultModelPath + meshName + ".obj";
	// File input object
	std::ifstream obj(filePath); // <-- Replace filename with your parameter
								 // Check for successful open
	if (!obj.is_open()) {
		LogText("--ERROR--//Cant find file.");
		return;
	}
	LogText("LOADING MODEL");
	LogText(meshName);
	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	std::vector<DirectX::XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);
		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			DirectX::XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);
			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the 9 face indices into an array
			unsigned int i[9];
			sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(vertCounter++);
			indices.push_back(vertCounter++);
			indices.push_back(vertCounter++);
		}
	}

	// Close
	obj.close();

	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
	if (vertCounter >= 0 && numberOfMeshes + 1 < MAX_NUM_MESHES) {
		int index = GetNextMeshIndex();
		if (index != -1) {
			AddMesh(meshName, &verts[0], vertCounter, &indices[0], vertCounter);
		}
	}
}

Mesh* Resources::AddMesh(std::string meshName, Vertex * vertices, int numVerts, UINT * indices, int newNumIndices)
{
	int index = GetNextMeshIndex();
	if (index != -1) {
		meshes[index] = new Mesh(&vertices[0], numVerts, &indices[0], newNumIndices, device);
		meshNameToIndex[index] = meshName;
		numberOfMeshes++;
		return meshes[index];
	}
	return nullptr;
}

int Resources::GetNextMeshIndex()
{
	if (numberOfMeshes + 1 >= MAX_NUM_MESHES) {
		LogText("--Max Meshes Reached--//Maximum unique number of meshes loaded. Consider loading less or increase the amount of mesh that can be stored.");
		return -1;
	}
	
	return numberOfMeshes;
}

//Finds the first mesh of the given name
int Resources::FindMesh(std::string meshName)
{
	for (int m = 0; m < numberOfMeshes; m++) {
		if (meshNameToIndex[m].compare(meshName) == 0) {
			return m;
		}
	}
	return -1;
}
