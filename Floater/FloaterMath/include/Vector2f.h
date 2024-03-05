#pragma once

#include "Common.h"
#include <cmath>
#include <compare>

namespace flt
{
	struct __declspec(dllexport) Vector2f
	{
		constexpr Vector2f() noexcept : m{ 0.0f, 0.0f } {}
		constexpr Vector2f(float x, float y) noexcept : m{ x, y } {}
		constexpr Vector2f(double x, double y) noexcept : m{ static_cast<float>(x), static_cast<float>(y) } {}
		constexpr Vector2f(Vector2f&&) noexcept = default;
		constexpr Vector2f(const Vector2f&) noexcept = default;
		constexpr Vector2f& operator=(Vector2f&&) noexcept = default;
		constexpr Vector2f& operator=(const Vector2f&) noexcept = default;
		~Vector2f() noexcept = default;

		// 두 수중 큰 값 * epsilon 값보다 차이가 적다면 같은것으로 간주.
		// x, y 우선순위로 비교.
		std::partial_ordering operator<=>(const Vector2f& rhs) const noexcept
		{
			std::partial_ordering order = std::partial_ordering::unordered;

			float epsilonX = std::fabsf(std::fmaxf(x, rhs.x)) * FLOAT_EPSILON;
			if (x - rhs.x < -epsilonX)
			{
				order = std::partial_ordering::less;
			}
			else if (x - rhs.x > epsilonX)
			{
				order = std::partial_ordering::greater;
			}
			else
			{
				float epsilonY = std::fabsf(std::fmaxf(y, rhs.y)) * FLOAT_EPSILON;
				if (y - rhs.y < -epsilonY)
				{
					order = std::partial_ordering::less;
				}
				else if (y - rhs.y > epsilonY)
				{
					order = std::partial_ordering::greater;
				}
				else
				{
					order = std::partial_ordering::equivalent;
				}
			}

			return order;
		}

		Vector2f& operator+=(const Vector2f& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vector2f& operator-=(const Vector2f& rhs) noexcept
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vector2f& operator*=(const float rhs) noexcept
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		Vector2f& operator/=(const float rhs) noexcept
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}
		Vector2f operator+(const Vector2f& rhs) const noexcept
		{
			return Vector2f(*this) += rhs;
		}
		Vector2f operator-(const Vector2f& rhs) const noexcept
		{
			return Vector2f(*this) -= rhs;
		}
		Vector2f operator*(const float rhs) const noexcept
		{
			return Vector2f(*this) *= rhs;
		}
		Vector2f operator/(const float rhs) const noexcept
		{
			return Vector2f(*this) /= rhs;
		}
		Vector2f operator-() const noexcept
		{
			return Vector2f(-x, -y);
		}

		float Norm() const noexcept
		{
			return sqrtf(x * x + y * y);
		}

		Vector2f& Normalize() noexcept
		{
			float norm = Norm();
			if (norm == 0)
			{
				return *this;
			}

			*this /= norm;
			return *this;
		}

		Vector2f Normalized() const noexcept
		{
			return Vector2f(*this).Normalize();
		}

		float Dot(const Vector2f& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y;
		}

	public:
		union
		{
			float m[2];
			struct { float x, y; };
			struct { float u, v; };
		};
	};
}
