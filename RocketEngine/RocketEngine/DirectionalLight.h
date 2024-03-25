#pragma once
#include "DLLExporter.h"
#include "Light.h"

namespace Rocket::Core
{
	class IDirectionalLight;
}

namespace Rocket
{
	class ROCKET_API DirectionalLight final : public Light
	{
	public:
		DirectionalLight();

	private:
		Core::IDirectionalLight* _directionalLight;
	};
}
