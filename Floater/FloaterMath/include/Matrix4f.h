#pragma once

//#include <DirectXMath.h>
#include <immintrin.h>
#include "Vector4f.h"


namespace flt
{
	struct __declspec(dllexport) Matrix4f final
	{
		constexpr Matrix4f() noexcept : v{ {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} } {}
		constexpr Matrix4f(
			float e00, float e01, float e02, float e03,
			float e10, float e11, float e12, float e13,
			float e20, float e21, float e22, float e23,
			float e30, float e31, float e32, float e33) :
			v{
				{e00, e01, e02, e03},
				{e10, e11, e12, e13},
				{e20, e21, e22, e23},
				{e30, e31, e32, e33}
			} {}
		constexpr Matrix4f(const Matrix4f&) noexcept = default;
		constexpr Matrix4f(Matrix4f&&) noexcept = default;
		constexpr Matrix4f& operator=(const Matrix4f&) noexcept = default;
		constexpr Matrix4f& operator=(Matrix4f&&) noexcept = default;
		~Matrix4f() noexcept = default;

		///DX 의존성 제거를 위한 주석처리
		//operator DirectX::XMMATRIX() const noexcept;

		Matrix4f& operator+=(const Matrix4f& rhs)
		{
			// 내부에서 sse 명령어 사용.
			v[0] += rhs.v[0];
			v[1] += rhs.v[1];
			v[2] += rhs.v[2];
			v[3] += rhs.v[3];
			return *this;

			////SSE version
			//v[0].m = _mm_add_ps(v[0].m, rhs.v[0].m);
			//v[1].m = _mm_add_ps(v[1].m, rhs.v[1].m);
			//v[2].m = _mm_add_ps(v[2].m, rhs.v[2].m);
			//v[3].m = _mm_add_ps(v[3].m, rhs.v[3].m);
			//return *this;
		}
		Matrix4f operator+(const Matrix4f& rhs) const
		{
			Matrix4f temp = *this;
			temp += rhs;
			return temp;
		}
		Matrix4f& operator-=(const Matrix4f& rhs)
		{
			// 내부에서 sse
			v[0] -= rhs.v[0];
			v[1] -= rhs.v[1];
			v[2] -= rhs.v[2];
			v[3] -= rhs.v[3];
			return *this;
		}
		Matrix4f operator-(const Matrix4f& rhs) const
		{
			Matrix4f temp = *this;
			temp -= rhs;
			return temp;
		}
		Matrix4f& operator*=(const Matrix4f& rhs) noexcept
		{
			Matrix4f temp = *this;

			//SSE version
			Vector4f tmp[4] =
			{
				{rhs.e[0][0], rhs.e[1][0], rhs.e[2][0], rhs.e[3][0]},
				{rhs.e[0][1], rhs.e[1][1], rhs.e[2][1], rhs.e[3][1]},
				{rhs.e[0][2], rhs.e[1][2], rhs.e[2][2], rhs.e[3][2]},
				{rhs.e[0][3], rhs.e[1][3], rhs.e[2][3], rhs.e[3][3]}
			};

			auto a = _mm_dp_ps(temp.v[0].m, tmp[0].m, 0xFF);

			//_mm_dp_ps 는 SSE4.1 이상에서만 사용 가능합니다.

			// 혹은 SSE 명령어를 이용해
			e[0][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, tmp[0].m, 0xFF));
			e[0][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, tmp[1].m, 0xFF));
			e[0][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, tmp[2].m, 0xFF));
			e[0][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, tmp[3].m, 0xFF));

			e[1][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, tmp[0].m, 0xFF));
			e[1][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, tmp[1].m, 0xFF));
			e[1][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, tmp[2].m, 0xFF));
			e[1][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, tmp[3].m, 0xFF));

			e[2][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, tmp[0].m, 0xFF));
			e[2][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, tmp[1].m, 0xFF));
			e[2][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, tmp[2].m, 0xFF));
			e[2][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, tmp[3].m, 0xFF));

			e[3][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, tmp[0].m, 0xFF));
			e[3][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, tmp[1].m, 0xFF));
			e[3][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, tmp[2].m, 0xFF));
			e[3][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, tmp[3].m, 0xFF));

			return *this;
		}
		Matrix4f operator*(const Matrix4f& rhs) const noexcept
		{
			Matrix4f temp =* this;
			temp *= rhs;
			return temp;
		}
		Matrix4f& operator*=(float rhs) noexcept
		{
			v[0] *= rhs;
			v[1] *= rhs;
			v[2] *= rhs;
			v[3] *= rhs;
			return *this;
		}
		Matrix4f operator*(float rhs) const noexcept
		{
			Matrix4f temp = *this;
			temp *= rhs;
			return temp;
		}
		Matrix4f& operator/=(float rhs) noexcept
		{
			v[0] /= rhs;
			v[1] /= rhs;
			v[2] /= rhs;
			v[3] /= rhs;
			return *this;
		}
		Matrix4f operator/(float rhs) const noexcept
		{
			Matrix4f temp = *this;
			temp /= rhs;
			return temp;
		}

		float Determinant()  const noexcept
		{
			float det
				= (e[0][3] * e[1][2] * e[2][1] * e[3][0])
				- (e[0][2] * e[1][3] * e[2][1] * e[3][0])
				- (e[0][3] * e[1][1] * e[2][2] * e[3][0])
				+ (e[0][1] * e[1][3] * e[2][2] * e[3][0])
				+ (e[0][2] * e[1][1] * e[2][3] * e[3][0])
				- (e[0][1] * e[1][2] * e[2][3] * e[3][0])
				- (e[0][3] * e[1][2] * e[2][0] * e[3][1])
				+ (e[0][2] * e[1][3] * e[2][0] * e[3][1])
				+ (e[0][3] * e[1][0] * e[2][2] * e[3][1])
				- (e[0][0] * e[1][3] * e[2][2] * e[3][1])
				- (e[0][2] * e[1][0] * e[2][3] * e[3][1])
				+ (e[0][0] * e[1][2] * e[2][3] * e[3][1])
				+ (e[0][3] * e[1][1] * e[2][0] * e[3][2])
				- (e[0][1] * e[1][3] * e[2][0] * e[3][2])
				- (e[0][3] * e[1][0] * e[2][1] * e[3][2])
				+ (e[0][0] * e[1][3] * e[2][1] * e[3][2])
				+ (e[0][1] * e[1][0] * e[2][3] * e[3][2])
				- (e[0][0] * e[1][1] * e[2][3] * e[3][2])
				- (e[0][2] * e[1][1] * e[2][0] * e[3][3])
				+ (e[0][1] * e[1][2] * e[2][0] * e[3][3])
				+ (e[0][2] * e[1][0] * e[2][1] * e[3][3])
				- (e[0][0] * e[1][2] * e[2][1] * e[3][3])
				- (e[0][1] * e[1][0] * e[2][2] * e[3][3])
				+ (e[0][0] * e[1][1] * e[2][2] * e[3][3]);

			return det;
		}
		float Determinant3x3() const
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
		Matrix4f Transpose()  const noexcept
		{
			return Matrix4f
			{
				e[0][0], e[1][0], e[2][0], e[3][0],
				e[0][1], e[1][1], e[2][1], e[3][1],
				e[0][2], e[1][2], e[2][2], e[3][2],
				e[0][3], e[1][3], e[2][3], e[3][3]
			};
		}
		Matrix4f Inverse() const noexcept
		{
			float det = this->Determinant();

			// det가 엡실론보다 작을 경우에는 역행렬이 존재하지 않는다.
			// 단위행렬을 리턴한다.
			float epsilon = FLOAT_EPSILON;
			if (det < epsilon && det > -epsilon)
			{
				//return Matrix4f
				//{
				//	1.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 1.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 1.0f, 0.0f,
				//	0.0f, 0.0f, 0.0f, 1.0f
				//};
				return Identity();
			}

			float invDet = 1.0f / det;
			Matrix4f metrix
			{
				e[1][1] * e[2][2] * e[3][3] + e[1][2] * e[2][3] * e[3][1] + e[1][3] * e[2][1] * e[3][2] - e[1][1] * e[2][3] * e[3][2] - e[1][2] * e[2][1] * e[3][3] - e[1][3] * e[2][2] * e[3][1],
				e[0][1] * e[2][3] * e[3][2] + e[0][2] * e[2][1] * e[3][3] + e[0][3] * e[2][2] * e[3][1] - e[0][1] * e[2][2] * e[3][3] - e[0][2] * e[2][3] * e[3][1] - e[0][3] * e[2][1] * e[3][2],
				e[0][1] * e[1][2] * e[3][3] + e[0][2] * e[1][3] * e[3][1] + e[0][3] * e[1][1] * e[3][2] - e[0][1] * e[1][3] * e[3][2] - e[0][2] * e[1][1] * e[3][3] - e[0][3] * e[1][2] * e[3][1],
				e[0][1] * e[1][3] * e[2][2] + e[0][2] * e[1][1] * e[2][3] + e[0][3] * e[1][2] * e[2][1] - e[0][1] * e[1][2] * e[2][3] - e[0][2] * e[1][3] * e[2][1] - e[0][3] * e[1][1] * e[2][2],
				e[1][0] * e[2][3] * e[3][2] + e[1][2] * e[2][0] * e[3][3] + e[1][3] * e[2][2] * e[3][0] - e[1][0] * e[2][2] * e[3][3] - e[1][2] * e[2][3] * e[3][0] - e[1][3] * e[2][0] * e[3][2],
				e[0][0] * e[2][2] * e[3][3] + e[0][2] * e[2][3] * e[3][0] + e[0][3] * e[2][0] * e[3][2] - e[0][0] * e[2][3] * e[3][2] - e[0][2] * e[2][0] * e[3][3] - e[0][3] * e[2][2] * e[3][0],
				e[0][0] * e[1][3] * e[3][2] + e[0][2] * e[1][0] * e[3][3] + e[0][3] * e[1][2] * e[3][0] - e[0][0] * e[1][2] * e[3][3] - e[0][2] * e[1][3] * e[3][0] - e[0][3] * e[1][0] * e[3][2],
				e[0][0] * e[1][2] * e[2][3] + e[0][2] * e[1][3] * e[2][0] + e[0][3] * e[1][0] * e[2][2] - e[0][0] * e[1][3] * e[2][2] - e[0][2] * e[1][0] * e[2][3] - e[0][3] * e[1][2] * e[2][0],
				e[1][0] * e[2][1] * e[3][3] + e[1][1] * e[2][3] * e[3][0] + e[1][3] * e[2][0] * e[3][1] - e[1][0] * e[2][3] * e[3][1] - e[1][1] * e[2][0] * e[3][3] - e[1][3] * e[2][1] * e[3][0],
				e[0][0] * e[2][3] * e[3][1] + e[0][1] * e[2][0] * e[3][3] + e[0][3] * e[2][1] * e[3][0] - e[0][0] * e[2][1] * e[3][3] - e[0][1] * e[2][3] * e[3][0] - e[0][3] * e[2][0] * e[3][1],
				e[0][0] * e[1][1] * e[3][3] + e[0][1] * e[1][3] * e[3][0] + e[0][3] * e[1][0] * e[3][1] - e[0][0] * e[1][3] * e[3][1] - e[0][1] * e[1][0] * e[3][3] - e[0][3] * e[1][1] * e[3][0],
				e[0][0] * e[1][3] * e[2][1] + e[0][1] * e[1][0] * e[2][3] + e[0][3] * e[1][1] * e[2][0] - e[0][0] * e[1][1] * e[2][3] - e[0][1] * e[1][3] * e[2][0] - e[0][3] * e[1][0] * e[2][1],
				e[1][0] * e[2][2] * e[3][1] + e[1][1] * e[2][0] * e[3][2] + e[1][2] * e[2][1] * e[3][0] - e[1][0] * e[2][1] * e[3][2] - e[1][1] * e[2][2] * e[3][0] - e[1][2] * e[2][0] * e[3][1],
				e[0][0] * e[2][1] * e[3][2] + e[0][1] * e[2][2] * e[3][0] + e[0][2] * e[2][0] * e[3][1] - e[0][0] * e[2][2] * e[3][1] - e[0][1] * e[2][0] * e[3][2] - e[0][2] * e[2][1] * e[3][0],
				e[0][0] * e[1][2] * e[3][1] + e[0][1] * e[1][0] * e[3][2] + e[0][2] * e[1][1] * e[3][0] - e[0][0] * e[1][1] * e[3][2] - e[0][1] * e[1][2] * e[3][0] - e[0][2] * e[1][0] * e[3][1],
				e[0][0] * e[1][1] * e[2][2] + e[0][1] * e[1][2] * e[2][0] + e[0][2] * e[1][0] * e[2][1] - e[0][0] * e[1][2] * e[2][1] - e[0][1] * e[1][0] * e[2][2] - e[0][2] * e[1][1] * e[2][0]
			};
			metrix *= invDet;

			return metrix;
		}
		static Matrix4f Identity() noexcept
		{
			Matrix4f temp
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};

			return temp;
		}

		// sse와 곱셈 시간 비교용 함수
		Matrix4f& Mul(const Matrix4f rhs)
		{
			Matrix4f temp = *this;
			e[0][0] = temp.e[0][0] * rhs.e[0][0] + temp.e[0][1] * rhs.e[1][0] + temp.e[0][2] * rhs.e[2][0] + temp.e[0][3] * rhs.e[3][0];
			e[0][1] = temp.e[0][0] * rhs.e[0][1] + temp.e[0][1] * rhs.e[1][1] + temp.e[0][2] * rhs.e[2][1] + temp.e[0][3] * rhs.e[3][1];
			e[0][2] = temp.e[0][0] * rhs.e[0][2] + temp.e[0][1] * rhs.e[1][2] + temp.e[0][2] * rhs.e[2][2] + temp.e[0][3] * rhs.e[3][2];
			e[0][3] = temp.e[0][0] * rhs.e[0][3] + temp.e[0][1] * rhs.e[1][3] + temp.e[0][2] * rhs.e[2][3] + temp.e[0][3] * rhs.e[3][3];

			e[1][0] = temp.e[1][0] * rhs.e[0][0] + temp.e[1][1] * rhs.e[1][0] + temp.e[1][2] * rhs.e[2][0] + temp.e[1][3] * rhs.e[3][0];
			e[1][1] = temp.e[1][0] * rhs.e[0][1] + temp.e[1][1] * rhs.e[1][1] + temp.e[1][2] * rhs.e[2][1] + temp.e[1][3] * rhs.e[3][1];
			e[1][2] = temp.e[1][0] * rhs.e[0][2] + temp.e[1][1] * rhs.e[1][2] + temp.e[1][2] * rhs.e[2][2] + temp.e[1][3] * rhs.e[3][2];
			e[1][3] = temp.e[1][0] * rhs.e[0][3] + temp.e[1][1] * rhs.e[1][3] + temp.e[1][2] * rhs.e[2][3] + temp.e[1][3] * rhs.e[3][3];

			e[2][0] = temp.e[2][0] * rhs.e[0][0] + temp.e[2][1] * rhs.e[1][0] + temp.e[2][2] * rhs.e[2][0] + temp.e[2][3] * rhs.e[3][0];
			e[2][1] = temp.e[2][0] * rhs.e[0][1] + temp.e[2][1] * rhs.e[1][1] + temp.e[2][2] * rhs.e[2][1] + temp.e[2][3] * rhs.e[3][1];
			e[2][2] = temp.e[2][0] * rhs.e[0][2] + temp.e[2][1] * rhs.e[1][2] + temp.e[2][2] * rhs.e[2][2] + temp.e[2][3] * rhs.e[3][2];
			e[2][3] = temp.e[2][0] * rhs.e[0][3] + temp.e[2][1] * rhs.e[1][3] + temp.e[2][2] * rhs.e[2][3] + temp.e[2][3] * rhs.e[3][3];

			e[3][0] = temp.e[3][0] * rhs.e[0][0] + temp.e[3][1] * rhs.e[1][0] + temp.e[3][2] * rhs.e[2][0] + temp.e[3][3] * rhs.e[3][0];
			e[3][1] = temp.e[3][0] * rhs.e[0][1] + temp.e[3][1] * rhs.e[1][1] + temp.e[3][2] * rhs.e[2][1] + temp.e[3][3] * rhs.e[3][1];
			e[3][2] = temp.e[3][0] * rhs.e[0][2] + temp.e[3][1] * rhs.e[1][2] + temp.e[3][2] * rhs.e[2][2] + temp.e[3][3] * rhs.e[3][2];
			e[3][3] = temp.e[3][0] * rhs.e[0][3] + temp.e[3][1] * rhs.e[1][3] + temp.e[3][2] * rhs.e[2][3] + temp.e[3][3] * rhs.e[3][3];

			return *this;
		}

		union
		{
			Vector4f v[4];
			float e[4][4];
		};
	};
}

