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

	void Draw(RenderInfo& renderInfo);
	void Update() override;

	Mesh* GetMesh() { return mesh; }
private:
	Render* render;
	Mesh* mesh;
	Material* material;
};