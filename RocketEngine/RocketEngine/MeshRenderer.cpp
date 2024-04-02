#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IMeshRenderer.h"

namespace Rocket
{
	MeshRenderer::MeshRenderer()
		: _graphicsRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateStaticModelRenderer())
	{

	}

// 	void StaticModelRenderer::SetMesh(eMeshType meshType)
// 	{
// 		_meshRenderer->LoadModel(meshType);
// 	}

	void MeshRenderer::LoadMesh(std::string fileName)
	{
		_graphicsRenderer->LoadMesh(fileName);
		_graphicsRenderer->BindTransform(gameObject->transform._rocketTransform);
	}

	void MeshRenderer::SetTexture(std::string fileName)
	{
		_graphicsRenderer->LoadTexture(fileName);
	}

	void MeshRenderer::BindTransform()
	{
		_graphicsRenderer->BindTransform(gameObject->transform._rocketTransform);
	}

	void MeshRenderer::UpdateRenderData()
	{
		_graphicsRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_graphicsRenderer->SetActive(gameObject->IsActive());
	}

}
