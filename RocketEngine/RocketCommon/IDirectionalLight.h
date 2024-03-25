#pragma once
#include "ILight.h"

namespace Rocket::Core
{
	class IDirectionalLight : public ILight
	{
	public:
		virtual ~IDirectionalLight() {}
	};
}
