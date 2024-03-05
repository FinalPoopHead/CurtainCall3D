#pragma once
#include "Vector3f.h"

namespace flt
{
	struct __declspec(dllexport) Matrix3f final
	{
		constexpr Matrix3f() noexcept : Matrix3f{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}
		constexpr Matrix3f(const Matrix3f& other) noexcept : Matrix3f{
			other.e[0][0], other.e[0][1], other.e[0][2],
			other.e[1][0], other.e[1][1], other.e[1][2],
			other.e[2][0], other.e[2][1], other.e[2][2] } {}
		constexpr Matrix3f(Matrix3f&& other) noexcept : Matrix3f{
			other.e[0][0], other.e[0][1], other.e[0][1],
			other.e[1][0], other.e[1][1], other.e[1][1],
			other.e[2][0], other.e[2][1], other.e[2][1] } {}
		constexpr Matrix3f(
			float e00, float e01, float e02,
			float e10, float e11, float e12,
			float e20, float e21, float e22) :
			v{ {e00, e01, e02}, {e10, e11, e12}, {e20, e21, e22} } {}

		constexpr Matrix3f& operator=(const Matrix3f& other) noexcept
		{
			e[0][0] = other.e[0][0];
			e[0][1] = other.e[0][1];
			e[0][2] = other.e[0][2];
			e[1][0] = other.e[1][0];
			e[1][1] = other.e[1][1];
			e[1][2] = other.e[1][2];
			e[2][0] = other.e[2][0];
			e[2][1] = other.e[2][1];
			e[2][2] = other.e[2][2];
			return *this;
		}
		constexpr Matrix3f& operator=(Matrix3f&& other) noexcept
		{
			e[0][0] = other.e[0][0];
			e[0][1] = other.e[0][1];
			e[0][2] = other.e[0][2];
			e[1][0] = other.e[1][0];
			e[1][1] = other.e[1][1];
			e[1][2] = other.e[1][2];
			e[2][0] = other.e[2][0];
			e[2][1] = other.e[2][1];
			e[2][2] = other.e[2][2];
			return *this;
		}

		Matrix3f& operator+=(const Matrix3f& rhs) noexcept
		{
			e[0][0] += rhs.e[0][0];
			e[0][1] += rhs.e[0][1];
			e[0][2] += rhs.e[0][2];
			e[1][0] += rhs.e[1][0];
			e[1][1] += rhs.e[1][1];
			e[1][2] += rhs.e[1][2];
			e[2][0] += rhs.e[2][0];
			e[2][1] += rhs.e[2][1];
			e[2][2] += rhs.e[2][2];
			return *this;
		}
		Matrix3f operator+(const Matrix3f& rhs) const noexcept
		{
			return Matrix3f(*this) += rhs;
		}
		Matrix3f& operator-=(const Matrix3f& rhs) noexcept
		{
			e[0][0] -= rhs.e[0][0];
			e[0][1] -= rhs.e[0][1];
			e[0][2] -= rhs.e[0][2];
			e[1][0] -= rhs.e[1][0];
			e[1][1] -= rhs.e[1][1];
			e[1][2] -= rhs.e[1][2];
			e[2][0] -= rhs.e[2][0];
			e[2][1] -= rhs.e[2][1];
			e[2][2] -= rhs.e[2][2];
			return *this;
		}
		Matrix3f operator-(const Matrix3f& rhs) const noexcept
		{
			return Matrix3f(*this) -= rhs;
		}
		Matrix3f& operator*=(const Matrix3f& rhs) noexcept
		{
			Matrix3f temp;
			for (int i = 0; i < 3; ++i)
			{
				temp.e[i][0] = e[i][0] * rhs.e[0][0] + e[i][1] * rhs.e[1][0] + e[i][2] * rhs.e[2][0];
				temp.e[i][1] = e[i][0] * rhs.e[0][1] + e[i][1] * rhs.e[1][1] + e[i][2] * rhs.e[2][1];
				temp.e[i][2] = e[i][0] * rhs.e[0][2] + e[i][1] * rhs.e[1][2] + e[i][2] * rhs.e[2][2];
			}
			return *this = temp;
		}
		Matrix3f operator*(const Matrix3f& rhs) const noexcept
		{
			return Matrix3f(*this) *= rhs;
		}
		Matrix3f& operator*=(float rhs) noexcept
		{
			e[0][0] *= rhs;
			e[0][1] *= rhs;
			e[0][2] *= rhs;
			e[1][0] *= rhs;
			e[1][1] *= rhs;
			e[1][2] *= rhs;
			e[2][0] *= rhs;
			e[2][1] *= rhs;
			e[2][2] *= rhs;
			return *this;
		}
		Matrix3f operator*(float rhs) const noexcept
		{
			return Matrix3f(*this) *= rhs;
		}
		Matrix3f& operator/=(float rhs) noexcept
		{
			e[0][0] /= rhs;
			e[0][1] /= rhs;
			e[0][2] /= rhs;
			e[1][0] /= rhs;
			e[1][1] /= rhs;
			e[1][2] /= rhs;
			e[2][0] /= rhs;
			e[2][1] /= rhs;
			e[2][2] /= rhs;
			return *this;
		}
		Matrix3f operator/(float rhs) const noexcept
		{
			return Matrix3f(*this) /= rhs;
		}

		float Determinant() const
		{
			float det
				= (e[0][0] * e[1][1] * e[2][2])
				+ (e[0][1] * e[1][2] * e[2][0])
				+ (e[0][2] * e[1][0] * e[2][1])
				- (e[0][2] * e[1][1] * e[2][0])
				- (e[0][1] * e[1][0] * e[2][2])
				- (e[0][0] * e[1][2] * e[2][1]);

			return det;
		}
		Matrix3f Transpose() const
		{
			return Matrix3f
			{
				e[0][0], e[1][0], e[2][0],
				e[0][1], e[1][1], e[2][1],
				e[0][2], e[1][2], e[2][2]
			};
		}
		Matrix3f Inverse() const
		{
			float det = Determinant();
			float epsilon = FLOAT_EPSILON;
			if (det < epsilon && det > -epsilon)
			{
				return Matrix3f::Identity();
			}

			float invDet = 1.0f / det;
			return Matrix3f
			{
				(e[1][1] * e[2][2] - e[1][2] * e[2][1])* invDet,
				(e[0][2] * e[2][1] - e[0][1] * e[2][2])* invDet,
				(e[0][1] * e[1][2] - e[0][2] * e[1][1])* invDet,

				(e[1][2] * e[2][0] - e[1][0] * e[2][2])* invDet,
				(e[0][0] * e[2][2] - e[0][2] * e[2][0])* invDet,
				(e[0][2] * e[1][0] - e[0][0] * e[1][2])* invDet,

				(e[1][0] * e[2][1] - e[1][1] * e[2][0])* invDet,
				(e[0][1] * e[2][0] - e[0][0] * e[2][1])* invDet,
				(e[0][0] * e[1][1] - e[0][1] * e[1][0])* invDet
			};
		}

		static Matrix3f Identity() noexcept
		{
			return Matrix3f{ 1.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 1.0f };
		}

		union
		{
			Vector3f v[3];
			float e[3][3];
		};
	};
}


