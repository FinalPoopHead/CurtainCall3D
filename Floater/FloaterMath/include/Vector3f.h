#pragma once

#include "Common.h"
#include <cmath>
#include <compare>


namespace flt
{
	struct Matrix3f;

	struct __declspec(dllexport) Vector3f
	{
		constexpr Vector3f() noexcept : m{ 0.0f, 0.0f, 0.0f } {}
		constexpr Vector3f(float x, float y, float z) noexcept : m{ x, y, z } {}
		constexpr Vector3f(double x, double y, double z) noexcept : m{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) } {}
		constexpr Vector3f(Vector3f&&) noexcept = default;
		constexpr Vector3f(const Vector3f&) noexcept = default;
		constexpr Vector3f& operator=(Vector3f&&) noexcept = default;
		constexpr Vector3f& operator=(const Vector3f&) noexcept = default;
		~Vector3f() noexcept = default;

		//operator Vector4f() const noexcept;

		bool operator==(const Vector3f& rhs) const noexcept;

		// 두 수중 큰 값 * epsilon 값보다 차이가 적다면 같은것으로 간주.
		// x, y, z 우선순위로 비교.
		std::partial_ordering operator<=>(const Vector3f& rhs) const noexcept
		{
			std::partial_ordering order = std::partial_ordering::unordered;

			float epsilonX = std::fabsf(std::fmaxf(x, rhs.x)) * FLOAT_EPSILON;
			float sub = x - rhs.x;
			if (sub < -epsilonX)
			{
				order = std::partial_ordering::less;
			}
			else if (sub > epsilonX)
			{
				order = std::partial_ordering::greater;
			}
			else
			{
				float epsilonY = std::fabsf(std::fmaxf(y, rhs.y)) * FLOAT_EPSILON;
				sub = y - rhs.y;
				if (sub < -epsilonY)
				{
					order = std::partial_ordering::less;
				}
				else if (sub > epsilonY)
				{
					order = std::partial_ordering::greater;
				}
				else
				{
					float epsilonZ = std::fabsf(std::fmaxf(z, rhs.z)) * FLOAT_EPSILON;
					sub = z - rhs.z;
					if (sub < -epsilonZ)
					{
						order = std::partial_ordering::less;
					}
					else if (sub > epsilonZ)
					{
						order = std::partial_ordering::greater;
					}
					else
					{
						order = std::partial_ordering::equivalent;
					}
				}
			}

			return order;
		}

		constexpr Vector3f& operator+=(const Vector3f& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		constexpr Vector3f& operator-=(const Vector3f& rhs) noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		constexpr Vector3f& operator*=(const float rhs) noexcept
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		constexpr Vector3f& operator/=(const float rhs) noexcept
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}
		constexpr Vector3f operator+(const Vector3f& rhs) const noexcept
		{
			return Vector3f(*this) += rhs;
		}
		constexpr Vector3f operator-(const Vector3f& rhs) const noexcept
		{
			return Vector3f(*this) -= rhs;
		}
		constexpr Vector3f operator*(const float rhs) const noexcept
		{
			return Vector3f(*this) *= rhs;
		}
		constexpr Vector3f operator/(const float rhs) const noexcept
		{
			return Vector3f(*this) /= rhs;
		}
		constexpr Vector3f& operator*=(const Matrix3f& rhs) noexcept;
		constexpr Vector3f operator*(const Matrix3f& rhs) const noexcept
		{
			return Vector3f(*this) *= rhs;
		}
		constexpr Vector3f operator-() const noexcept
		{
			return Vector3f(-x, -y, -z);
		}

		float Norm() const noexcept
		{
			return sqrtf(x * x + y * y + z * z);
		}

		Vector3f& Normalize() noexcept
		{
			float norm = Norm();
			if (norm == 0)
			{
				return *this;
			}

			*this /= norm;
			return *this;
		}

		Vector3f Normalized() const noexcept
		{
			return Vector3f(*this).Normalize();
		}

		float Dot(const Vector3f& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		Vector3f Cross(const Vector3f& rhs) const noexcept
		{
			return Vector3f(y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x);
		}

	public:
		union
		{
			float m[3];
			struct { float x, y, z; };
			struct { float r, g, b; };
		};

		static Vector3f Lerp(const Vector3f& v0, const Vector3f& v1, float t) noexcept;

		static Vector3f Zero() noexcept
		{
			return Vector3f(0.0f, 0.0f, 0.0f);
		}
	};

}
