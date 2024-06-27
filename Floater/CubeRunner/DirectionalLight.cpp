#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: _lightComponent(nullptr)
	, _direction(90.0f, 0.0f, 0.0f)
{
	_lightComponent = AddComponent<flt::DirectionalLight>(true);
	_lightComponent->SetDirection(_direction);
}
