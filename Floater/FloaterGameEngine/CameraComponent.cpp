#include "./include/internal/CameraComponent.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererCommon/include/Camera.h"


flt::CameraComponent::CameraComponent() 
	: _rendererObject(new RendererObject{ _isDraw })
	, _renderer(*GameEngine::Instance()->GetRenderer())
	, _hObject()
	, _isDraw(true)
{
	_rendererObject->camera = new flt::Camera(&_gameObject->transform);
}

flt::CameraComponent::~CameraComponent()
{
	delete _rendererObject;
}

uint32 flt::CameraComponent::SetIndex(uint32 priority)
{
	int oldIndex = _rendererObject->camera->priority;
	_rendererObject->camera->priority = (int)priority;

	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
	
	return (uint32)oldIndex;
}

flt::Matrix4f flt::CameraComponent::GetViewMatrix() const
{
	return _rendererObject->camera->GetViewMatrix();
}

flt::Matrix4f flt::CameraComponent::GetProjectionMatrix() const
{
	return _rendererObject->camera->GetProjectionMatrix();
}

void flt::CameraComponent::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;
}

void flt::CameraComponent::OnEnable()
{
	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(*_rendererObject);
	}
}

void flt::CameraComponent::Update(float deltaSecond)
{
	_isDraw = true;
}

void flt::CameraComponent::PostRender()
{
	_isDraw = false;
}

void flt::CameraComponent::OnDestroy()
{
	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
	}

	delete _rendererObject->camera;
	_rendererObject->camera = nullptr;
}
