﻿#pragma once

#include <immintrin.h>
//#include <cmath>
#include "FloaterMathDefine.h"
#include "Vector3f.h"



namespace flt
{
	struct Matrix4f;

	struct FLOATER_MATH_API Vector4f
	{
	public:
		[[nodiscard]] static Vector4f Lerp(const Vector4f& a, const Vector4f& b, float t) noexcept;

	public:
		//constexpr Vector4f() noexcept : m{ .m128_f32{0.0f, 0.0f, 0.0f, 0.0f} } {}
		//constexpr Vector4f(float x, float y, float z, float w) noexcept : m{ .m128_f32{x, y, z, w} } {}
		//constexpr Vector4f(double x, double y, double z, double w) noexcept : m{ .m128_f32{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)} } {}
		constexpr Vector4f() noexcept : e{0.0f, 0.0f, 0.0f, 0.0f} {}
		constexpr Vector4f(float x, float y, float z, float w) noexcept : e{x, y, z, w} {}
		constexpr Vector4f(double x, double y, double z, double w) noexcept : e{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)} {}
		constexpr Vector4f(__m128 m) noexcept : m(m) {}
		Vector4f(const Vector3f& v, float w) noexcept;
		constexpr Vector4f(Vector4f&&) noexcept = default;
		constexpr Vector4f(const Vector4f&) noexcept = default;
		constexpr Vector4f& operator=(Vector4f&&) noexcept = default;
		constexpr Vector4f& operator=(const Vector4f&) noexcept = default;
		~Vector4f() noexcept = default;

		[[nodiscard]] bool isSimiller(const Vector4f& rhs, float epsilon = FLOAT_EPSILON) const noexcept
		{
			float eps = std::fmaxf(epsilon * fmaxf(fabsf(x), fabsf(rhs.x)), epsilon);
			bool ret = fabsf(x - rhs.x) <= eps;

			eps = std::fmaxf(epsilon * fmaxf(fabsf(y), fabsf(rhs.y)), epsilon);
			ret &= fabsf(y - rhs.y) <= eps;

			eps = std::fmaxf(epsilon * fmaxf(fabsf(z), fabsf(rhs.z)), epsilon);
			ret &= fabsf(z - rhs.z) <= eps;

			eps = std::fmaxf(epsilon * fmaxf(fabsf(w), fabsf(rhs.w)), epsilon);
			ret &= fabsf(w - rhs.w) <= eps;

			return ret;
		}

		[[nodiscard]] bool operator==(const Vector4f& rhs) const noexcept
		{
			float eps = std::fmaxf(FLOAT_EPSILON * fmaxf(fabsf(x), fabsf(rhs.x)), FLOAT_EPSILON);
			bool ret = fabsf(x - rhs.x) <= eps;

			eps = std::fmaxf(FLOAT_EPSILON * fmaxf(fabsf(y), fabsf(rhs.y)), FLOAT_EPSILON);
			ret &= fabsf(y - rhs.y) <= eps;

			eps = std::fmaxf(FLOAT_EPSILON * fmaxf(fabsf(z), fabsf(rhs.z)), FLOAT_EPSILON);
			ret &= fabsf(z - rhs.z) <= eps;

			eps = std::fmaxf(FLOAT_EPSILON * fmaxf(fabsf(w), fabsf(rhs.w)), FLOAT_EPSILON);
			ret &= fabsf(w - rhs.w) <= eps;

			return ret;
		}
		[[nodiscard]] std::partial_ordering operator<=>(const Vector4f& rhs) const noexcept
		{
			std::partial_ordering order = std::partial_ordering::unordered;

			float epsilonX = std::fmaxf(std::fabsf(std::fmaxf(x, rhs.x)) * FLOAT_EPSILON, FLOAT_EPSILON);
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
				float epsilonY = std::fmaxf(std::fabsf(std::fmaxf(y, rhs.y)) * FLOAT_EPSILON, FLOAT_EPSILON);
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
					float epsilonZ = std::fmaxf(std::fabsf(std::fmaxf(z, rhs.z)) * FLOAT_EPSILON, FLOAT_EPSILON);
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
						float epsilonW = std::fmaxf(std::fabsf(std::fmaxf(w, rhs.w)) * FLOAT_EPSILON, FLOAT_EPSILON);
						sub = w - rhs.w;
						if (sub < -epsilonW)
						{
							order = std::partial_ordering::less;
						}
						else if (sub > epsilonW)
						{
							order = std::partial_ordering::greater;
						}
						else
						{
							order = std::partial_ordering::equivalent;
						}
					}
				}
			}

			return order;
		}

		[[nodiscard]] explicit operator Vector3f() const noexcept;
		[[nodiscard]] explicit operator __m128() const noexcept;

		Vector4f& operator+=(const Vector4f& rhs) noexcept
		{
			m = _mm_add_ps(m, rhs.m);
			return *this;
		}
		Vector4f& operator-=(const Vector4f& rhs) noexcept
		{
			m = _mm_sub_ps(m, rhs.m);
			return *this;
		}
		Vector4f& operator*=(const Vector4f& rhs) noexcept
		{
			// 헤밀턴 곱

			Vector4f temp = *this;
			w = temp.w * rhs.w - temp.x * rhs.x - temp.y * rhs.y - temp.z * rhs.z;
			x = temp.w * rhs.x + temp.x * rhs.w + temp.y * rhs.z - temp.z * rhs.y;
			y = temp.w * rhs.y - temp.x * rhs.z + temp.y * rhs.w + temp.z * rhs.x;
			z = temp.w * rhs.z + temp.x * rhs.y - temp.y * rhs.x + temp.z * rhs.w;

			return *this;
		}
		Vector4f& operator*=(const Matrix4f& rhs) noexcept;
		Vector4f& operator*=(const float rhs) noexcept
		{
			m = _mm_mul_ps(m, _mm_set1_ps(rhs));
			return *this;
		}
		Vector4f& operator/=(const float rhs) noexcept
		{
			m = _mm_div_ps(m, _mm_set1_ps(rhs));
			return *this;
		}
		[[nodiscard]] Vector4f operator+(const Vector4f& rhs) const noexcept
		{
			return Vector4f(*this) += rhs;
		}
		[[nodiscard]] Vector4f operator-(const Vector4f& rhs) const noexcept
		{
			return Vector4f(*this) -= rhs;
		}
		[[nodiscard]] Vector4f operator*(const Vector4f& rhs) const noexcept
		{
			return Vector4f(*this) *= rhs;
		}
		[[nodiscard]] Vector4f operator*(const Matrix4f& rhs) const noexcept
		{
			return Vector4f(*this) *= rhs;
		}
		[[nodiscard]] Vector4f operator*(const float rhs) const noexcept
		{
			return Vector4f(*this) *= rhs;
		}
		[[nodiscard]] Vector4f operator/(const float rhs) const noexcept
		{
			return Vector4f(*this) /= rhs;
		}
		[[nodiscard]] Vector4f operator-() const noexcept
		{
			return Vector4f(-x, -y, -z, -w);
		}

		[[nodiscard]] float Norm() const noexcept
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		[[nodiscard]] float NormPow() const noexcept
		{
			return x * x + y * y + z * z + w * w;
		}

		Vector4f& Normalize() noexcept
		{
			float norm = Norm();
			if (norm == 0)
			{
				return *this;
			}

			return *this /= norm;
		}
		[[nodiscard]] Vector4f Normalized() const noexcept
		{
			return Vector4f(*this).Normalize();
		}
		[[nodiscard]] float Vector3Norm() const noexcept
		{
			return sqrt(x * x + y * y + z * z);
		}
		Vector4f& Vector3Normalize() noexcept
		{
			float tmp = this->w;
			float norm = Vector3Norm();

			if (norm < 0.000001f)
			{
				return *this;
			}

			*this /= norm;
			this->w = tmp;
			return *this;
		}
		[[nodiscard]] Vector4f Vector3Normalized() const noexcept
		{
			return Vector4f(*this).Vector3Normalize();
		}
		[[nodiscard]] constexpr Vector4f Vector3Cross(const Vector4f& rhs) const noexcept
		{
			return Vector4f(
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x,
				0.0f
			);
		}
		[[nodiscard]] float Vector3Dot(const Vector4f& rhs) const noexcept
		{
			//return _mm_cvtss_f32(_mm_dp_ps(m, rhs.m, 0x71));
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}
		[[nodiscard]] float Dot(const Vector4f& rhs) const noexcept
		{
			//return _mm_cvtss_f32(_mm_dp_ps(m, rhs.m, 0xff));
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}

		union
		{
			__m128 m;
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
			float e[4];
		};
	};
}

