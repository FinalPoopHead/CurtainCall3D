#include "DirectionalLight.h"
#include "../RocketCommon/RocketTransform.h"

namespace Rocket::Core
{
	DirectionalLight::DirectionalLight()
		: _transform(nullptr),
		_diffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
		_ambientColor(0.1f, 0.1f, 0.1f, 0.1f),
		_specularColor(1.0f, 1.0f, 1.0f, 1.0f),
		_specularPower(1.0f)
	{

	}

	Vector3 DirectionalLight::GetForward() const
	{
		return _transform->GetForward();
	}

}
