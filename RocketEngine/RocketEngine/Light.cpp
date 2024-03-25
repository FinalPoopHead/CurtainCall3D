#include "Light.h"
#include "../RocketCommon/ILight.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/RocketTransform.h"

namespace Rocket
{
	// _light의 실체는 이것을 상속받은 구체적인 Light 컴포넌트들을 생성할때 bind한다.
	Light::Light()
		: _light(nullptr),
		_diffuseColor(1.0f, 1.0f, 1.0f, 1.0f),
		_ambientColor(0.1f, 0.1f, 0.1f, 1.0f),
		_specularColor(1.0f, 1.0f, 1.0f, 1.0f),
		_specularPower(1.0f)
	{

	}

	void Light::BindTransform()
	{
		_light->BindTransform(gameObject->transform._rocketTransform);
	}

	void Light::SetDiffuseColor(Color diffuse)
	{
		_diffuseColor = diffuse;
		_light->SetDiffuseColor(diffuse);
	}

	void Light::SetAmbientColor(Color ambient)
	{
		_ambientColor = ambient;
		_light->SetAmbientColor(ambient);
	}

	void Light::SetSpecularColor(Color specular)
	{
		_specularColor = specular;
		_light->SetSpecularColor(specular);

	}

	void Light::SetSpecularPower(float power)
	{
		_specularPower = power;
		_light->SetSpecularPower(power);
	}
}
