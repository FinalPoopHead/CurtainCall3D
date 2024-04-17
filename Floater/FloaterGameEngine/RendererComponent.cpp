#include "./include/RendererComponent.h"
#include "./include/GameObject.h"
#include "./include/Scene.h"
#include "./include/GameEngine.h"
#include "../FloaterRendererCommon/include/RawNode.h"


flt::RendererComponent::RendererComponent(GameObject* gameObject) :
	ComponentBase<RendererComponent>(gameObject),
	_rendererObject(gameObject->transform, _isDraw),
	_renderer(*GameEngine::Instance()->GetRenderer()), 
	_hObject(), 
	_isDraw(true)
{
}

flt::RendererComponent::~RendererComponent()
{

}

void flt::RendererComponent::OnEnable()
{
	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(_rendererObject);
	}
}

void flt::RendererComponent::Update(float deltaSecond)
{
	_isDraw = true;
}

void flt::RendererComponent::EndDraw()
{
	_isDraw = false;
}

void flt::RendererComponent::OnDestroy()
{
	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
	}
}

void flt::RendererComponent::SetRawNode(RawNode* rawNode)
{
	_rendererObject.node = rawNode;
}
