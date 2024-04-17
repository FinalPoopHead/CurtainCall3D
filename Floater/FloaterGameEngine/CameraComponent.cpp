﻿#include "./include/CameraComponent.h"
#include "./include/GameObject.h"
#include "./include/GameEngine.h"
#include "../FloaterRendererCommon/include/Camera.h"


flt::CameraComponent::CameraComponent(GameObject* gameObject) : 
	ComponentBase<CameraComponent>(gameObject),
	_rendererObject(gameObject->transform, _isDraw),
	_renderer(*GameEngine::Instance()->GetRenderer()),
	_hObject(),
	_isDraw(true)
{
	_rendererObject.camera = new flt::Camera(&gameObject->transform);
}

void flt::CameraComponent::OnEnable()
{
	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(_rendererObject);
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