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
	_isDraw(true),
	_isRegisted(false)
{
}

flt::RendererComponent::~RendererComponent()
{
	delete _rendererObject;
}

void flt::RendererComponent::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;

	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(*_rendererObject);
		_isRegisted = true;
	}
}

void flt::RendererComponent::OnEnable()
{
	_isDraw = true;
}

void flt::RendererComponent::OnDisable()
{
	_isDraw = false;
}

void flt::RendererComponent::Update(float deltaSecond)
{

}

void flt::RendererComponent::EndDraw()
{

}

void flt::RendererComponent::OnDestroy()
{
	_isDraw = false;

	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
		_isRegisted = false;
		_hObject = 0;
	}
}

void flt::RendererComponent::SetRawNode(RawNode* rawNode)
{
	_rendererObject->node = rawNode;
}

void flt::RendererComponent::SetFilePath(std::wstring path)
{
	RenderableBuilder builder(path);
	_resource.Set(builder);
	_rendererObject->node = _resource.Get()->node;

	if (_isRegisted)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}
