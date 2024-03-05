#include "SpotLight.h"

#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{

	SpotLight::SpotLight()
		: _range(1.0f),
		_spot(1.0f),
		_attenuation(1.0f, 1.0f, 1.0f)
	{

	}

// 	Rocket::Core::SpotLightData SpotLight::GetLightData() const
// 	{
// 		Rocket::Core::SpotLightData data;
// 		data.ambient = _ambient;
// 		data.diffuse = _diffuse;
// 		data.specular = _specular;
// 		data.position = gameObject->transform.GetLocalPosition();
// 		data.range = _range;
// 		data.direction = gameObject->transform.GetForward();
// 		data.spot = _spot;
// 		data.attenuation = _attenuation;
// 		data.tPad = 666.0f;
// 
// 		return data;
// 	}
	
	float SpotLight::GetRange() const
	{
		return _range;
	}

	void SpotLight::SetRange(float range)
	{
		_range = range;
	}

	float SpotLight::GetSpot() const
	{
		return _spot;
	}

	void SpotLight::SetSpot(float spot)
	{
		_spot = spot;
	}

	Vector3 SpotLight::GetAttenuation() const
	{
		return _attenuation;
	}

	void SpotLight::SetAttenuation(Vector3 attenuation)
	{
		_attenuation = attenuation;
	}

}
