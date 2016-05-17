#pragma once
#include "Mesh.h"
#include "Material.h"
#include "SpriteFont.h"

struct TextureResource {
	std::string name;
	ID3D11ShaderResourceView* srv;
};

struct MaterialResource {
	std::string name;
	Material* material;
};

struct FontResource {
	std::string name;
	SpriteFont* font;
};


class Resources
{

public:
	//Model file formats
	static const char FILE_FORMAT_OBJ[];
	//Image file formats
	static const char FILE_FORMAT_TGA[];
	static const char FILE_FORMAT_DDS[];
	static const char FILE_FORMAT_PNG[];
	static const char FILE_FORMAT_JPG[];
	//Text file formats
	static const char FILE_FORMAT_SPRITE_FONT[];

	const static unsigned int MAX_NUM_MESHES = 50;//Max number of meshes we can load
	const static unsigned int MAX_NUM_TEXTURES = 200;//Max number of textures we can load
	const static unsigned int MAX_NUM_MATERIALS = 100;//Max number of materials we can hold
	const static unsigned int MAX_NUM_FONTS = 50;//Max number of fonts we can hold

	Resources(ID3D11Device* newDevice, ID3D11DeviceContext* newDeviceContext);
	~Resources();

	//Mesh Stuff
	//TODO: refactor
	Mesh* GetMeshIfLoaded(const char* meshName);//Returns the mesh only if it is loaded
	Mesh* GetMeshAndLoadIfNotFound(const char* meshName);
	bool IsMeshLoaded(const char* meshName);
	void LoadMesh(std::string meshName);//Load a mesh for latter
	Mesh* AddMesh(std::string meshName, Vertex* vertices, int numVerts, UINT* indices, int newNumIndices);
	int GetNextMeshIndex();
	int FindMesh(std::string meshName);

	//Texture Stuff
	ID3D11ShaderResourceView* GetTextureIfLoaded(const char* textureName);
	bool IsTextureLoaded(const char* textureName);
	ID3D11ShaderResourceView* LoadTexture(std::string textureName, std::string format);
	ID3D11ShaderResourceView* LoadTextureFindFormat(std::string textureName);
	int FindTextureIndex(std::string textureName);

	//Material Stuff
	//Creates a material with the sampler state, shaders, loads the texture, and possibly the normal to. The shader resource name is equal to the texture name in this case 
	Material* CreateMaterial(SimpleVertexShader* vert, SimpleGeometryShader* geo, SimplePixelShader* pixel, ID3D11SamplerState* sampler, std::string textrureName);
	Material* CreateMaterial(SimpleVertexShader* vert, SimpleGeometryShader* geo, SimplePixelShader* pixel, ID3D11SamplerState* sampler, std::string textrure1Name, std::string textrure2Name);
	Material* CreateMaterial(SimpleVertexShader* vert, SimpleGeometryShader* geo, SimplePixelShader* pixel, ID3D11SamplerState* sampler, std::string textrure1Name, std::string textrure2Name, std::string textrure3Name);
	Material* GetMaterial(std::string materialName);
	int GetMaterialIndex(std::string materialName);

	//Font stuff
	SpriteFont* LoadSpriteFont(std::string fileName);
	SpriteFont* GetSpriteFont(std::string fileName);
private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	//TODO: move some of this info to a mesh resource instead of this
	std::string defaultModelPath;
	Mesh* meshes[MAX_NUM_MESHES];
	std::string* meshNameToIndex;
	unsigned int numberOfMeshes;

	std::string defaultTexturePath;
	TextureResource* textures;
	unsigned int numberOfTextures;

	MaterialResource* materials;
	unsigned int numberOfMaterials;

	std::string defaultSpriteFontPath;
	FontResource* fonts;
	unsigned int numberOfFonts;
};

