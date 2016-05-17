#include "DrawnMesh.h"
#include "Render.h"
#include "Transform.h"
#include "Logger.h"

DrawnMesh::DrawnMesh()
	:Component()
{
	render = nullptr;
	mesh = nullptr;
}

DrawnMesh::DrawnMesh(Render* newRender, Mesh* newMesh, Material* newMaterial)
	:Component()
{
	render = newRender;
	mesh = newMesh;
	material = newMaterial;
}

DrawnMesh::~DrawnMesh()
{
	//Don't delete the mesh, the mesh will handle itself
}

#include "Entity.h"

//This is just temporary, I still have to figure out how exactly I want to do rendering
void DrawnMesh::Draw(RenderInfo& renderInfo)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	if (material != nullptr) {
		material->PrepareMaterial(renderInfo, GetTransform()/*,individualVertexInfo, individualPixelInfo*/);//, individualVertexInfo, individualPixelInfo
	}
	else {
		//LogText("--WARRNING no material--//A drawn mesh is rendering without a material. Things will not work well.");
	}

	if (renderInfo.currentMesh != mesh) {
		renderInfo.deviceContext->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer(), &stride, &offset);
		renderInfo.deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		renderInfo.currentMesh = mesh;
	}
	renderInfo.deviceContext->DrawIndexed(mesh->GetNumberOfIndices(), 0, 0);
}

void DrawnMesh::Update(float dt)
{
	Component::Update(dt);
	if (render == nullptr) return;
	render->AddToRenderList(*this);
}
