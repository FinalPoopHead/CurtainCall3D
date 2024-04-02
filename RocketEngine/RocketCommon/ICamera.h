#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	class ICamera
	{
	public:
		virtual void SetNearZ(float nearZ) = 0;
		virtual void SetFarZ(float farZ) = 0;
		virtual void SetAspect(float aspect) = 0;
		virtual void SetFOVY(float fov) = 0;
		virtual void SetNearHeight(float height) = 0;
		virtual void SetFarHeight(float height) = 0;
		virtual void SetAsMainCamera() = 0;
		virtual void BindTransform(RocketTransform* transform) = 0;
	};
}
