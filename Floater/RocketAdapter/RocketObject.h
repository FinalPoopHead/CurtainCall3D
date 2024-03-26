#pragma once
#include "../FloaterRendererCommon/include/Transform.h"
#include "./RocketCommon/RocketTransform.h"

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
		RocketObject() :
			rocketTransform(),
			transform(nullptr),
			camera(nullptr),
			renderer(nullptr),
			rkModel(nullptr),
			isRegisted(false) {}

		Rocket::Core::RocketTransform rocketTransform;
		Transform* transform;
		Rocket::Core::ICamera* camera;
		Rocket::Core::IDynamicModelRenderer* renderer;
		Rocket::Core::RawModel* rkModel;

		bool isRegisted;
	};
}


