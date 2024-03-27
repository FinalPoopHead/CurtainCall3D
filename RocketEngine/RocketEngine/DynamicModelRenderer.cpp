#include "DynamicModelRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IDynamicModelRenderer.h"

namespace Rocket
{
	DynamicModelRenderer::DynamicModelRenderer()
		: _graphicsComponent(Core::GraphicsSystem::Instance().GetFactory()->CreateDynamicModelRenderer())
	{

	}

	void DynamicModelRenderer::LoadModel(std::string fileName)
	{
		_graphicsComponent->LoadModel(fileName);

		// TODO : 이거 CreateModelRenderer로 제작한 거 아니면 문제생길 수 있다. ex) 부모로 다른 오브젝트가 있는 상태에서 LoadModel 하는 경우.
		//			얘는 내가 Node버퍼에 넣을 데이터들과 같은 계층구조를 갖고있는 녀석이면 된다..
		Transform* rootTransform = &(gameObject->transform);
		while (rootTransform->GetParent() != nullptr)
		{
			rootTransform = rootTransform->GetParent();
		}
		_graphicsComponent->BindTransform(rootTransform->_rocketTransform);
	}

	void DynamicModelRenderer::SetTexture(std::string fileName)
	{
		_graphicsComponent->LoadTexture(fileName);
	}

	void DynamicModelRenderer::UpdateRenderData()
	{
		_graphicsComponent->SetWorldTM(gameObject->transform.GetWorldTM());
	}
}
