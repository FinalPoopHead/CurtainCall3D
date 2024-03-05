#include <cmath>
#include "./include/Quaternion.h"
#include "../FloaterUtil/include/FloaterMacro.h"


namespace flt
{

	Quaternion::Quaternion(const Vector3f& euler, AxisOrder order /*= AxisOrder::YXZ*/)
		: Vector4f(0.0f, 0.0f, 0.0f, 1.0f)
	{
		SetEuler(euler, order);
	}

	void Quaternion::SetEuler(float degreeX, float degreeY, float degreeZ, AxisOrder order) noexcept
	{
		float radX = DegToRad(degreeX * 0.5f);
		float radY = DegToRad(degreeY * 0.5f);
		float radZ = DegToRad(degreeZ * 0.5f);

		switch (order)
		{
			case flt::Quaternion::AxisOrder::XYZ:
			{
				float cos1 = cosf(radX);
				float sin1 = sinf(radX);
				float cos2 = cosf(radY);
				float sin2 = sinf(radY);
				float cos3 = cosf(radZ);
				float sin3 = sinf(radZ);

				w = cos1 * cos2 * cos3 - sin1 * sin2 * sin3;
				x = sin1 * cos2 * cos3 + sin2 * sin3 * cos1;
				y = sin2 * cos1 * cos3 - sin1 * sin3 * cos2;
				z = sin1 * sin2 * cos3 + sin3 * cos1 * cos2;
			}
			break;
			case flt::Quaternion::AxisOrder::XZY:
			{
				float cos1 = cosf(radX);
				float sin1 = sinf(radX);
				float cos2 = cosf(radZ);
				float sin2 = sinf(radZ);
				float cos3 = cosf(radY);
				float sin3 = sinf(radY);

				w = sin1 * sin2 * sin3 + cos1 * cos2 * cos3;
				x = sin1 * cos2 * cos3 - sin2 * sin3 * cos1;
				y = sin3 * cos1 * cos2 - sin1 * sin2 * cos3;
				z = sin1 * sin3 * cos2 + sin2 * cos1 * cos3;
			}
			break;
			case flt::Quaternion::AxisOrder::YXZ:
			{
				//// 일단 YXZ 순서로 구현
				float cos1 = cosf(radY);
				float sin1 = sinf(radY);
				float cos2 = cosf(radX);
				float sin2 = sinf(radX);
				float cos3 = cosf(radZ);
				float sin3 = sinf(radZ);

				w = sin1 * sin2 * sin3 + cos1 * cos2 * cos3;
				x = sin1 * sin3 * cos2 + sin2 * cos1 * cos3;
				y = sin1 * cos2 * cos3 - sin2 * sin3 * cos1;
				z = sin3 * cos1 * cos2 - sin1 * sin2 * cos3;
			}
			break;
			case flt::Quaternion::AxisOrder::YZX:
			{
				float cos1 = cosf(radY);
				float sin1 = sinf(radY);
				float cos2 = cosf(radZ);
				float sin2 = sinf(radZ);
				float cos3 = cosf(radX);
				float sin3 = sinf(radX);

				w = cos1 * cos2 * cos3 - sin1 * sin2 * sin3;
				x = sin1 * sin2 * cos3 + sin3 * cos1 * cos2;
				y = sin1 * cos2 * cos3 + sin2 * sin3 * cos1;
				z = sin2 * cos1 * cos3 - sin1 * sin3 * cos2;
			}
			break;
			case flt::Quaternion::AxisOrder::ZXY:
			{
				float cos1 = cosf(radZ);
				float sin1 = sinf(radZ);
				float cos2 = cosf(radX);
				float sin2 = sinf(radX);
				float cos3 = cosf(radY);
				float sin3 = sinf(radY);

				w = cos1 * cos2 * cos3 - sin1 * sin2 * sin3;
				x = sin2 * cos1 * cos3 - sin1 * sin3 * cos2;
				y = sin1 * sin2 * cos3 + sin3 * cos1 * cos2;
				z = sin1 * cos2 * cos3 + sin2 * sin3 * cos1;
			}
			break;
			case flt::Quaternion::AxisOrder::ZYX:
			{
				float cos1 = cosf(radZ);
				float sin1 = sinf(radZ);
				float cos2 = cosf(radY);
				float sin2 = sinf(radY);
				float cos3 = cosf(radX);
				float sin3 = sinf(radX);

				w = sin1 * sin2 * sin3 + cos1 * cos2 * cos3;
				x = sin3 * cos1 * cos2 - sin1 * sin2 * cos3;
				y = sin1 * sin3 * cos2 + sin2 * cos1 * cos3;
				z = sin1 * cos2 * cos3 - sin2 * sin3 * cos1;
			}
			break;
			default:
				ASSERT(false, "Invalid AxisOrder");
				break;
		}

		Normalize();
	}

	void Quaternion::SetEuler(const Vector3f& euler, AxisOrder order) noexcept
	{
		SetEuler(euler.x, euler.y, euler.z, order);
	}

	void Quaternion::SetAxisAngle(const Vector3f& axis, float radian) noexcept
	{
		Vector3f normalizedAxis = axis.Normalized();
		float sinHalfAngle = sinf(radian * 0.5f);
		x = normalizedAxis.x * sinHalfAngle;
		y = normalizedAxis.y * sinHalfAngle;
		z = normalizedAxis.z * sinHalfAngle;
		w = cosf(radian * 0.5f);
	}

	Vector3f Quaternion::GetEuler() const noexcept
	{
		// 일단 YXZ 순서로 구현
		float sRcP = 2.0f * (w * z + x * y);  // sinRoll * cosPitch
		float cRcP = 1.0f - 2.0f * (z * z + x * x); // cosRoll * cosPitch
		float roll = atan2(sRcP, cRcP);

		float sP = 2.0f * (w * x - y * z); // sinPitch
		float pitch = 0.0f;

		// 아크 사인 함수의 정의역이 -1 ~ 1이므로, 이를 벗어나는 경우를 처리해줘야 한다.
		if (sP < -1.0f)
		{
			pitch = -PI<float> / 2.0f;
		}
		else if (sP > 1.0f)
		{
			pitch = PI<float> / 2.0f;
		}
		else
		{
			pitch = asinf(sP);
		}

		float sYcP = 2.0f * (w * y + x * z); // sinYaw * cosPitch
		float cYcP = 1.0f - 2.0f * (x * x + y * y); // cosYaw * cosPitch
		float yaw = atan2(sYcP, cYcP);

		return Vector3f(RadToDeg(pitch), RadToDeg(yaw), RadToDeg(roll));
	}

	flt::Vector4f Quaternion::GetAxisAngle() const noexcept
	{
		float angle = 2.0f * acosf(w);
		float s = sqrtf(1.0f - w * w);
		if (s < 0.001f)
		{
			return Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			return Vector4f(x / s, y / s, z / s, angle);
		}
	}

}
