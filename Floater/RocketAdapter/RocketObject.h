#pragma once
#include "../FloaterRendererCommon/include/Transform.h"

namespace Rocket
{
	namespace Core
	{
		class ICamera;
		class IDynamicModelRenderer;
		struct RawModel;
	}
}

namespace flt
{
	struct RocketObject
	{
		Transform* transform;
		Rocket::Core::ICamera* camera;
		Rocket::Core::IDynamicModelRenderer* renderer;
		Rocket::Core::RawModel* rkModel;
	};
}


