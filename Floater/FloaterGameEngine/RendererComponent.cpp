#include "./include/internal/RendererComponent.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterRendererCommon/include/RawNode.h"


flt::RendererComponent::RendererComponent() :
	_rendererObject(new RendererObject{ _isDraw }),
	_renderer(*GameEngine::Instance()->GetRenderer()), 
	_hObject(), 
	_isDraw(true)
{
}

flt::RendererComponent::~RendererComponent()
{
	delete _rendererObject;
}

void flt::RendererComponent::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;
}

void flt::RendererComponent::OnEnable()
{
	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(*_rendererObject);
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
	_rendererObject->node = rawNode;
}
