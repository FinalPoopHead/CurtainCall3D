#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: _lightComponent(nullptr)
	, _direction(0.0f, 0.0f, 1.0f)
{
	_lightComponent = AddComponent<flt::DirectionalLight>(true);
}

void DirectionalLight::Update(float deltaTime)
{
	_direction.x = 140.0f;

	_lightComponent->SetDirection({_direction});
}
