#include "Resources.h"
#include <vector>
#include <fstream>
#include "Logger.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

const char Resources::FILE_FORMAT_OBJ[] = ".obj";

const char Resources::FILE_FORMAT_TGA[] = ".tga";
const char Resources::FILE_FORMAT_DDS[] = ".dds";
const char Resources::FILE_FORMAT_PNG[] = ".png";
const char Resources::FILE_FORMAT_JPG[] = ".jpg";

const char Resources::FILE_FORMAT_SPRITE_FONT[] = ".spritefont";

Resources::Resources(ID3D11Device* newDevice, ID3D11DeviceContext* newDeviceContext)
{
	device = newDevice;
	deviceContext = newDeviceContext;

	defaultModelPath = "Assets/Models/";
	numberOfMeshes = 0;
	meshNameToIndex = new std::string[MAX_NUM_MESHES];

	defaultTexturePath = "Assets/Textures/";
	textures = new TextureResource[MAX_NUM_TEXTURES];
	for (unsigned int t = 0; t < MAX_NUM_TEXTURES; ++t) {
		textures[t].srv = nullptr;
		textures[t].name = "";
	}
	numberOfTextures = 0;

	materials = new MaterialResource[MAX_NUM_MATERIALS];
	numberOfMaterials = 0;
	for (unsigned int m = 0; m < MAX_NUM_MATERIALS; ++m) {
		materials[m].material = nullptr;
		materials[m].name = "";
	}

	defaultSpriteFontPath = "Assets/Fonts/";
	fonts = new FontResource[MAX_NUM_FONTS];
	numberOfMaterials = 0;
	for (unsigned int f = 0; f < MAX_NUM_FONTS; ++f) {
		fonts[f].font = nullptr;
		fonts[f].name = "";
	}

	numberOfFonts = 0;
}


Resources::~Resources()
{
	//Handle clean up of meshes
	for (unsigned int m = 0; m < MAX_NUM_MESHES; m++) {
		if (meshes[m] != nullptr) {
			delete meshes[m];
			meshes[m] = nullptr;
		}
	}
	delete[] meshNameToIndex;

	//Handle cleanup of textures 
	//IMPORTANT for textures release the shader resource views
	for (unsigned int t = 0; t < MAX_NUM_TEXTURES; t++) {
		if (textures[t].srv != nullptr) {
			textures[t].srv->Release();
			textures[t].srv = nullptr;
		}
	}

	if (textures != nullptr) {
		delete[] textures;
		textures = nullptr;
	}

	//Handle material clean up
	for (int m = 0; m < MAX_NUM_MATERIALS; ++m) {
		if (materials[m].material != nullptr) {
			delete materials[m].material;
			materials[m].material = nullptr;
		}
	}
	if (materials != nullptr) {
		delete[] materials;
		materials = nullptr;
	}

	for (int f = 0; f < MAX_NUM_FONTS; ++f) {
		if (fonts[f].font != nullptr) {
			delete fonts[f].font;
			fonts[f].font = nullptr;
		}
	}
	if (fonts != nullptr) {
		delete[] fonts;
		fonts = nullptr;
	}
}
#pragma region Mesh

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
	std::string filePath = defaultModelPath + meshName + FILE_FORMAT_OBJ;
	// File input object
	std::ifstream obj(filePath); // <-- Replace filename with your parameter
								 // Check for successful open
	if (!obj.is_open()) {
		LogText("--ERROR loading Model--//Cant find the model file. Path: " + filePath);
		return;
	}
	LogText("LOADING MODEL: " + filePath);
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

	Vertex* tempVerts = new Vertex[verts.size()]();
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		tempVerts[i] = verts[i];
	}

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
			AddMesh(meshName, tempVerts, vertCounter, &indices[0], vertCounter);
		}
	}

	//delete tempVerts;
}

Mesh* Resources::AddMesh(std::string meshName, Vertex * vertices, int numVerts, UINT * indices, int newNumIndices)
{
	int index = GetNextMeshIndex();
	if (index != -1) {
		meshes[index] = new Mesh(vertices, numVerts, &indices[0], newNumIndices, device);
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
	for (unsigned int m = 0; m < numberOfMeshes; m++) {
		if (meshNameToIndex[m].compare(meshName) == 0) {
			return m;
		}
	}
	return -1;
}

#pragma endregion

#pragma region Texture
ID3D11ShaderResourceView* Resources::GetTextureIfLoaded(const char * textureName)
{
	int index = FindTextureIndex(textureName);
	if (index != -1) {
		return textures[index].srv;
	}
	return nullptr;
}

bool Resources::IsTextureLoaded(const char * textureName)
{
	return FindTextureIndex(textureName) != -1;
}

ID3D11ShaderResourceView* Resources::LoadTexture(std::string textureName, std::string format)
{
	//Don't load an already loaded texture
	int index = FindTextureIndex(textureName);
	if (index != -1) {
		return textures[index].srv;
	}

	//Make sure we have space for another texture
	if (numberOfTextures + 1 >= MAX_NUM_TEXTURES) {
		LogText("--Texture not Loaded--//You have reached your texture loading cap. Load less textures or increase the max number of textures.");
		return nullptr;
	}

	std::string texturePath = defaultTexturePath + textureName + format;
	LogText("LOADING TEXTURE: " + texturePath);

	//a hacky way to convert to a wide char, I would eventually like to find a better solution
	const char* texturePathC = texturePath.c_str();
	wchar_t buffer[256] = { 0 };
	MultiByteToWideChar(0, 0, texturePathC, strlen(texturePathC), buffer, strlen(texturePathC));

	TextureResource newTextureResource;
	newTextureResource.name = textureName;
	newTextureResource.srv = nullptr;

	//Check to see if we have as DDS texture and load it differntly instead
	if (format.compare(Resources::FILE_FORMAT_DDS) == 0) {
		DirectX::CreateDDSTextureFromFile(device, deviceContext, buffer, NULL, &newTextureResource.srv);
	}
	else {
		DirectX::CreateWICTextureFromFile(device, deviceContext, buffer, NULL, &newTextureResource.srv);
	}

	//If the texture didn't load
	if (newTextureResource.srv == nullptr) {
		LogText("--ERROR loading Texture--//Failed to find the texture file. Path: " + texturePath);
		//delete newTextureResource;
		return nullptr;
	}

	//Sucessfully loaded the texture
	textures[numberOfTextures] = newTextureResource;
	numberOfTextures += 1;
	return newTextureResource.srv;
}

ID3D11ShaderResourceView * Resources::LoadTextureFindFormat(std::string textureName)
{
	//tries to load PNGs, then JPGS
	ID3D11ShaderResourceView* possibleTexture = LoadTexture(textureName, Resources::FILE_FORMAT_PNG);
	if (possibleTexture != nullptr) return possibleTexture;
	possibleTexture = LoadTexture(textureName, Resources::FILE_FORMAT_JPG);
	if (possibleTexture != nullptr) return possibleTexture;

	return nullptr;
}

int Resources::FindTextureIndex(std::string textureName)
{
	for (unsigned int t = 0; t < numberOfTextures; t++) {
		if (textures[t].name.compare(textureName) == 0) {
			return t;
		}
	}
	return -1;
}
#pragma endregion

#pragma region Material 

Material* Resources::CreateMaterial(SimpleVertexShader * vert, SimpleGeometryShader* geo, SimplePixelShader * pixel, ID3D11SamplerState * sampler, std::string textrureName)
{
	return Resources::CreateMaterial(vert, geo, pixel, sampler, textrureName, "", "");
}

Material * Resources::CreateMaterial(SimpleVertexShader * vert, SimpleGeometryShader* geo, SimplePixelShader * pixel, ID3D11SamplerState * sampler, std::string textrure1Name, std::string textrure2Name)
{
	return Resources::CreateMaterial(vert, geo, pixel, sampler, textrure1Name, textrure2Name, "");
}

//TODO:have a better way of handeling materials, as in if you try to create a material with a duplicate fists texture name it won't be created and will instead return the first one
Material* Resources::CreateMaterial(SimpleVertexShader * vert, SimpleGeometryShader* geo, SimplePixelShader * pixel, ID3D11SamplerState * sampler,
	std::string textrure1Name,
	std::string textrure2Name,
	std::string textrure3Name)
{
	int index = GetMaterialIndex(textrure1Name);
	if (index != -1) {
		return materials[index].material;
	}

	if (numberOfMaterials + 1 >= MAX_NUM_MATERIALS) {
		LogText("--Not Creating Material--//Max number of materials reached. Create less or increase the max number of materials to hold.");
		return nullptr;
	}

	//This might be changed out in the future, for a scanning system that tries to load the best file format works and then tries to load fallbacks
	ID3D11ShaderResourceView* textures[Material::MAX_NUM_TEXTURES];
	unsigned int numberOfTextures = 1;
	textures[0] = LoadTextureFindFormat(textrure1Name);
	if (textrure2Name != "") {
		textures[1] = LoadTextureFindFormat(textrure2Name);
		numberOfTextures += 1;
	}
	if (textrure3Name != "") {
		textures[2] = LoadTextureFindFormat(textrure3Name);
		numberOfTextures += 1;
	}
	if (textures[0] == nullptr) {
		LogText("--Not creating Material--//No diffuse map was loaded. So no material will be created: " + textrure1Name);
		return nullptr;
	}
	Material* newMaterial = new Material(vert, geo, pixel, textures, numberOfTextures, sampler);
	materials[numberOfMaterials].material = newMaterial;
	materials[numberOfMaterials].name = textrure1Name;
	numberOfMaterials += 1;
	return newMaterial;
}

Material* Resources::GetMaterial(std::string materialName)
{
	int index = GetMaterialIndex(materialName);
	if (index != -1) {
		return materials[index].material;
	}
	return nullptr;
}

int Resources::GetMaterialIndex(std::string materialName)
{
	for (unsigned int m = 0; m < numberOfMaterials; ++m) {
		if (materials[m].name.compare(materialName) == 0) {
			return m;
		}
	}
	return -1;
}

SpriteFont* Resources::LoadSpriteFont(std::string fileName)
{
	SpriteFont* isFontLoaded = GetSpriteFont(fileName);
	if (isFontLoaded != nullptr) return isFontLoaded;
	if (numberOfFonts + 1 >= MAX_NUM_FONTS) {
		LogText("--Not Loading Font--//Max number of fonts reached. Load less or increase the max number of fonts to hold.");
		return nullptr;
	}

	std::string fontPath = defaultSpriteFontPath + fileName + FILE_FORMAT_SPRITE_FONT;
	LogText("LOADING FONT: " + fileName);

	//a hacky way to convert to a wide char, I would eventually like to find a better solution
	const char* texturePathC = fontPath.c_str();
	wchar_t buffer[256] = { 0 };
	MultiByteToWideChar(0, 0, texturePathC, strlen(texturePathC), buffer, strlen(texturePathC));

	SpriteFont* newFont = new SpriteFont(device, buffer);
	fonts[numberOfFonts].font = newFont;
	fonts[numberOfFonts].name = fileName;
	numberOfFonts += 1;
	return newFont;
}

SpriteFont * Resources::GetSpriteFont(std::string fileName)
{
	for (unsigned int f = 0; f < numberOfFonts; ++f) {
		if (fonts[f].name.compare(fileName) == 0) {
			return fonts[f].font;
		}
	}
	return nullptr;
}

#pragma endregion
