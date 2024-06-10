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
		class ISpriteRenderer;
		class ITextRenderer;
		struct RawModel;
	}
}

namespace flt
{
	struct RocketObject
	{
		RocketObject(const bool& isDraw) :
			rocketTransform(),
			transform(nullptr),
			camera(nullptr),
			renderer(nullptr),
			staticModelRenderer(nullptr),
			spriteRenderer(nullptr),
			textRenderer(nullptr),
			rkModel(nullptr),
			isRegisted(false),
			isDraw(isDraw) {}

		Rocket::Core::RocketTransform rocketTransform;
		Transform* transform;
		Rocket::Core::ICamera* camera;
		Rocket::Core::IDynamicModelRenderer* renderer;
		Rocket::Core::IStaticModelRenderer* staticModelRenderer;
		Rocket::Core::ISpriteRenderer* spriteRenderer;
		Rocket::Core::ITextRenderer* textRenderer;
		Rocket::Core::RawModel* rkModel;

		bool isRegisted;
		const bool& isDraw;
	};
}


