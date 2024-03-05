#pragma once

//#include <immintrin.h>

#include "Vector4f.h"
#include "Common.h"

namespace flt
{
	struct Vector3f;
}

namespace flt
{
	struct __declspec(dllexport) Quaternion : public Vector4f
	{
		enum class  AxisOrder
		{
			XYZ, XZY, YXZ, YZX, ZXY, ZYX
		};

		constexpr Quaternion() noexcept : Vector4f(0.0f, 0.0f, 0.0f, 1.0f) {}
		Quaternion(float x, float y, float z, float w) noexcept : Vector4f(Vector4f{ x, y, z, w }.Normalize()) {}
		Quaternion(const Vector3f& euler, AxisOrder order = AxisOrder::YXZ);
		constexpr Quaternion(const Vector4f& v) noexcept : Vector4f(v) {}
		Quaternion(const Vector3f& axis, float radian) noexcept : Vector4f(axis.Normalized()* sin(radian * 0.5f), cos(radian * 0.5f)) {}
		Quaternion(const Vector4f& axis, float radian) noexcept : Quaternion((Vector3f)axis, radian) {}
		constexpr Quaternion(Quaternion&&) noexcept = default;
		constexpr Quaternion(const Quaternion&) noexcept = default;

		~Quaternion() noexcept = default;

		constexpr Quaternion& operator=(Quaternion&&) noexcept = default;
		constexpr Quaternion& operator=(const Quaternion&) noexcept = default;

		void SetEuler(float degreeX, float degreeY, float defreeZ, AxisOrder order = AxisOrder::YXZ) noexcept;
		void SetEuler(const Vector3f& euler, AxisOrder order = AxisOrder::YXZ) noexcept;
		void SetAxisAngle(const Vector3f& axis, float radian) noexcept;

		Vector3f GetEuler() const noexcept;
		Vector4f GetAxisAngle() const noexcept;

		static Quaternion Slerp(Quaternion q1, Quaternion q2, float t) noexcept
		{
			float dot = q1.Dot(q2);

			if (dot < 0.0f)
			{
				q1 = -q1;
				dot = -dot;
			}

			if (dot > 1.0f)
			{
				Quaternion result = q1 + (q2 - q1) * t;
				result.Normalize();
				return result;
			}

			float theta0 = acos(dot);
			float theta = theta0 * t;
			float sinTheta = sin(theta);
			float sinTheta0 = sin(theta0);
			float s0 = cos(theta) - dot * sinTheta / sinTheta0;
			float s1 = sinTheta / sinTheta0;
			return q1 * s0 + q2 * s1;
		}
	};
}
