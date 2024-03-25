#include "DirectionalLight.h"
#include "../RocketCommon/IDirectionalLight.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/RocketTransform.h"

namespace Rocket
{
	DirectionalLight::DirectionalLight()
		: _directionalLight(Core::GraphicsSystem::Instance().GetFactory()->CreateDirectionalLight())
	{
		_light = _directionalLight;
	}
}
