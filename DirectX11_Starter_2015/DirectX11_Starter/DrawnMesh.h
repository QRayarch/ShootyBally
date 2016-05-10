#pragma once
#include "Mesh.h"
#include "Component.h"
#include <d3d11.h>
#include "SimpleShader.h"
#include "Material.h"

struct RenderInfo;
class Render;

class DrawnMesh : public Component
{
public:
	DrawnMesh();
	DrawnMesh(Render* newRender, Mesh* newMesh, Material* newMaterial);
	~DrawnMesh();

	virtual void Draw(RenderInfo& renderInfo);
	void Update() override;

	Mesh* GetMesh() { return mesh; }
	void SetMaterial(Material* newMaterial) { material = newMaterial; }
	Material* GetMaterial() { return material; }
	//MaterialInfo& GetIndividualVertexInfo() { return individualVertexInfo; }
	//MaterialInfo& GetIndividualPixelInfo() { return individualPixelInfo; }
private:
	Render* render;
	Mesh* mesh;
	Material* material;
	//MaterialInfo individualVertexInfo;
	//MaterialInfo individualPixelInfo;
};