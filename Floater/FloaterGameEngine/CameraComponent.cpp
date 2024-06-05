#include "./include/internal/CameraComponent.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/GameEngine.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererCommon/include/Camera.h"


flt::CameraComponent::CameraComponent() : 
	_rendererObject(new RendererObject{ _isDraw }),
	_renderer(*GameEngine::Instance()->GetRenderer()),
	_hObject(),
	_isDraw(true)
{
	
}

void flt::CameraComponent::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;
	_rendererObject->camera = new flt::Camera(&_gameObject->transform);
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

void flt::CameraComponent::EndDraw()
{
	_isDraw = false;
}

void flt::CameraComponent::OnDestroy()
{
	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
	}
}
