#pragma once

#include <immintrin.h>
//#include <cmath>
#include "Vector3f.h"


namespace flt
{
	struct Matrix4f;

	struct __declspec(dllexport) Vector4f
	{
	public:
		constexpr Vector4f() noexcept : m{ .m128_f32{0.0f, 0.0f, 0.0f, 0.0f} } {}
		constexpr Vector4f(float x, float y, float z, float w) noexcept : m{ .m128_f32{x, y, z, w} } {}
		constexpr Vector4f(double x, double y, double z, double w) noexcept : m{ .m128_f32{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)} } {}
		constexpr Vector4f(__m128 m) noexcept : m(m) {}
		Vector4f(const Vector3f& v, float w) noexcept;
		constexpr Vector4f(Vector4f&&) noexcept = default;
		constexpr Vector4f(const Vector4f&) noexcept = default;
		constexpr Vector4f& operator=(Vector4f&&) noexcept = default;
		constexpr Vector4f& operator=(const Vector4f&) noexcept = default;
		~Vector4f() noexcept = default;

		explicit operator Vector3f() const noexcept;
		explicit operator __m128() const noexcept;

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
		Vector4f operator+(const Vector4f& rhs) const noexcept
		{
			return Vector4f(*this) += rhs;
		}
		Vector4f operator-(const Vector4f& rhs) const noexcept
		{
			return Vector4f(*this) -= rhs;
		}
		Vector4f operator*(const Vector4f& rhs) const noexcept
		{
			return Vector4f(*this) *= rhs;
		}
		Vector4f operator*(const Matrix4f& rhs) const noexcept
		{
			return Vector4f(*this) *= rhs;
		}
		Vector4f operator*(const float rhs) const noexcept
		{
			return Vector4f(*this) *= rhs;
		}
		Vector4f operator/(const float rhs) const noexcept
		{
			return Vector4f(*this) /= rhs;
		}
		Vector4f operator-() const noexcept
		{
			return Vector4f(-x, -y, -z, -w);
		}

		float Norm() const noexcept
		{
			return sqrt(x * x + y * y + z * z + w * w);
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
		Vector4f Normalized() const noexcept
		{
			return Vector4f(*this).Normalize();
		}
		float Vector3Norm() const noexcept
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
		Vector4f Vector3Normalized() const noexcept
		{
			return Vector4f(*this).Vector3Normalize();
		}
		constexpr Vector4f Vector3Cross(const Vector4f& rhs) const noexcept
		{
			return Vector4f(
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x,
				0.0f
			);
		}
		float Vector3Dot(const Vector4f& rhs) const noexcept
		{
			return _mm_cvtss_f32(_mm_dp_ps(m, rhs.m, 0x71));
			//__m128 tmp = _mm_mul_ps(m, rhs.m);
			//return tmp.m128_f32[0] + tmp.m128_f32[1] + tmp.m128_f32[2];
		}
		float Dot(const Vector4f& rhs) const noexcept
		{
			return _mm_cvtss_f32(_mm_dp_ps(m, rhs.m, 0xff));
		}

		union
		{
			__m128 m;
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
		};
	};
}

