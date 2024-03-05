#include "DirectionalLight.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	DirectionalLight::DirectionalLight()
		: _intensity(1.0f)
	{

	}

// 	Rocket::Core::DirectionalLightData DirectionalLight::GetLightData() const
// 	{
// 		Rocket::Core::DirectionalLightData data;
// 		data.ambient = _ambient;
// 		data.diffuse = _diffuse;
// 		data.specular = _specular;
// 		data.direction = gameObject->transform.GetForward();
// 		data.intensity = _intensity;
// 
// 		return data;
// 	}

	float DirectionalLight::GetIntensity() const
	{
		return _intensity;
	}

	void DirectionalLight::SetIntensity(float intensity)
	{
		_intensity = intensity;
	}
}
