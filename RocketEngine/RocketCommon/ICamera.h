#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class ICamera
	{
	public:
		virtual void SetPositionAndRotation(const Vector3& pos, const Quaternion& rot) = 0;
		virtual void SetWorldTM(const Matrix& matrix) = 0;
		virtual void SetNearZ(float nearZ) = 0;
		virtual void SetFarZ(float farZ) = 0;
		virtual void SetAspect(float aspect) = 0;
		virtual void SetFOVY(float fov) = 0;
		virtual void SetNearHeight(float height) = 0;
		virtual void SetFarHeight(float height) = 0;
		virtual void SetAsMainCamera() = 0;
	};
}