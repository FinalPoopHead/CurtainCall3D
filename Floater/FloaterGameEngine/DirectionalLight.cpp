#include "./include/internal/DirectionalLight.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "./include/internal/GameEngine.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"


flt::DirectionalLight::DirectionalLight()
	: _direction()
	, _diffuseColor(1.0f, 1.0f, 1.0f)
	, _specularColor(1.0f, 1.0f, 1.0f)
	, _ambientColor(0.0f, 0.0f, 0.0f)
	, _intensity(0.0f)
	, _renderer(*GameEngine::Instance()->GetRenderer())
	, _rendererObject(new RendererObject{ _isDraw })
	, _hObject()
	, _isDraw(false)
	, _isRegisted(true)
{
	_rendererObject->light = new Light();
	_rendererObject->light->type = Light::Type::directional;
}

flt::DirectionalLight::~DirectionalLight()
{
	delete _rendererObject;
}

void flt::DirectionalLight::SetDirection(flt::Vector3f dir)
{
	_rendererObject->light->direction = Quaternion{dir};
}

void flt::DirectionalLight::SetDiffuseColor(flt::Vector3f color)
{
	_diffuseColor = color;
}

void flt::DirectionalLight::SetSpecularColor(flt::Vector3f color)
{
	_specularColor = color;
}

void flt::DirectionalLight::SetAmbientColor(flt::Vector3f color)
{
	_ambientColor = color;
}

void flt::DirectionalLight::SetIntensity(float intensity)
{
	_intensity = intensity;
}

void flt::DirectionalLight::OnCreate()
{
	_rendererObject->transform = &_gameObject->transform;
	if (!_hObject)
	{
		_hObject = _renderer.RegisterObject(*_rendererObject);
		_isRegisted = true;
	}
}

void flt::DirectionalLight::OnEnable()
{
	_isDraw = true;
}

void flt::DirectionalLight::OnDisable()
{
	_isDraw = false;
}

void flt::DirectionalLight::OnDestroy()
{
	_isDraw = false;

	if (_hObject)
	{
		_renderer.DeregisterObject(_hObject);
		_isRegisted = false;
		_hObject = 0;
	}

	delete _rendererObject->light;
	_rendererObject->light = nullptr;
}
