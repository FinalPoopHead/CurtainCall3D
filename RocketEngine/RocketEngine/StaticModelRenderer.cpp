#include "StaticModelRenderer.h"
#include "GraphicsSystem.h"
#include "ResourceSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IStaticModelRenderer.h"

namespace Rocket
{
	StaticModelRenderer::StaticModelRenderer()
		: _graphicsRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateStaticModelRenderer())
	{

	}

	void StaticModelRenderer::LoadModel(std::string fileName)
	{
		Core::ResourceSystem::Instance().GetModel(fileName);
		_graphicsRenderer->LoadModel(fileName);
		_graphicsRenderer->BindTransform(gameObject->transform._rocketTransform);
	}

	void StaticModelRenderer::SetBaseColorTexture(std::string fileName)
	{
		_graphicsRenderer->LoadBaseColorTexture(fileName);
	}

	void StaticModelRenderer::SetNormalTexture(std::string fileName)
	{
		_graphicsRenderer->LoadNormalTexture(fileName);
	}

	void StaticModelRenderer::SetMetallicTexture(std::string fileName)
	{
		_graphicsRenderer->LoadMetallicTexture(fileName);
	}

	void StaticModelRenderer::SetRoughnessTexture(std::string fileName)
	{
		_graphicsRenderer->LoadRoughnessTexture(fileName);
	}

	void StaticModelRenderer::SetAOTexture(std::string fileName)
	{
		_graphicsRenderer->LoadAOTexture(fileName);
	}

	void StaticModelRenderer::SetMetallic(float value)
	{
		_graphicsRenderer->SetMetallic(value);
	}

	void StaticModelRenderer::SetRoughness(float value)
	{
		_graphicsRenderer->SetRoughness(value);
	}

	void StaticModelRenderer::BindTransform()
	{
		_graphicsRenderer->BindTransform(gameObject->transform._rocketTransform);
	}

	void StaticModelRenderer::UpdateRenderData()
	{
		_graphicsRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_graphicsRenderer->SetActive(gameObject->IsActive());
	}

}
