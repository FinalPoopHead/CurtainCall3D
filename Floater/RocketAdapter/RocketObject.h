#pragma once
#include "../FloaterRendererCommon/include/Transform.h"
#include "./RocketCommon/RocketTransform.h"

namespace Rocket
{
	namespace Core
	{
		class ICamera;
		class IDynamicModelRenderer;
		class IStaticModelRenderer;
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
			staticMeshRenderer(nullptr),
			rkModel(nullptr),
			isRegisted(false) {}

		Rocket::Core::RocketTransform rocketTransform;
		Transform* transform;
		Rocket::Core::ICamera* camera;
		Rocket::Core::IDynamicModelRenderer* renderer;
		Rocket::Core::IStaticModelRenderer* staticMeshRenderer;
		Rocket::Core::RawModel* rkModel;

		bool isRegisted;
	};
}


