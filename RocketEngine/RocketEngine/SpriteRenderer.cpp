#include "SpriteRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/ISpriteRenderer.h"

namespace Rocket
{
	SpriteRenderer::SpriteRenderer()
		: _spriteRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateSpriteRenderer())
	{

	}

	void SpriteRenderer::UpdateRenderData()
	{
		_spriteRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_spriteRenderer->SetActive(gameObject->IsActive());
	}

	void SpriteRenderer::SetPath(std::string path)
	{
		_spriteRenderer->SetImage(path);
	}
}
