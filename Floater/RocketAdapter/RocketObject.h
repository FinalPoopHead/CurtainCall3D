﻿#pragma once
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
		class IDirectionalLight;
		struct RawModel;
		struct RawNode;
	}
}

namespace flt
{
	struct RocketObject
	{
		RocketObject(const bool& isDraw)
			: rocketTransform()
			, transform(nullptr)
			, camera(nullptr)
			, renderer(nullptr)
			, staticModelRenderer(nullptr)
			, spriteRenderer(nullptr)
			, textRenderer(nullptr)
			, rkModel(nullptr)
			, directionalLight(nullptr)
			, lightTransform(nullptr)
			, lightRotation(nullptr)
			, isRegisted(false)
			, isDraw(isDraw) {}

		~RocketObject();

		void RemoveRKNodeRecursive(Rocket::Core::RawNode* node);
		void RemoveRKTransformRecursive(Rocket::Core::RocketTransform* transform);

		Rocket::Core::RocketTransform rocketTransform;
		Transform* transform;
		Rocket::Core::ICamera* camera;
		Rocket::Core::IDynamicModelRenderer* renderer;
		Rocket::Core::IStaticModelRenderer* staticModelRenderer;
		Rocket::Core::ISpriteRenderer* spriteRenderer;
		Rocket::Core::ITextRenderer* textRenderer;
		Rocket::Core::RawModel* rkModel;
		Rocket::Core::IDirectionalLight* directionalLight;
		Rocket::Core::RocketTransform* lightTransform;
		Quaternion* lightRotation;

		bool isRegisted;
		const bool& isDraw;
	};
}


