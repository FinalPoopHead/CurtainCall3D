#pragma once

//#include <DirectXMath.h>
#include <immintrin.h>
#include "Common.h"
#include "Vector4f.h"
#include "../../FloaterUtil/include/FloaterMacro.h"


namespace flt
{
	struct FLOATER_MATH_API Matrix4f final
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

		bool operator==(const Matrix4f& rhs) const noexcept
		{
			return v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2] && v[3] == rhs.v[3];
		}

		std::partial_ordering operator<=>(const Matrix4f& rhs) const noexcept
		{
			std::partial_ordering order = std::partial_ordering::unordered;

			order = v[0] <=> rhs.v[0];

			if (order != std::partial_ordering::equivalent)
			{
				return order;
			}

			order = v[1] <=> rhs.v[1];

			if (order != std::partial_ordering::equivalent)
			{
				return order;
			}

			order = v[2] <=> rhs.v[2];

			if (order != std::partial_ordering::equivalent)
			{
				return order;
			}

			order = v[3] <=> rhs.v[3];

			return order;
		}

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
			// Vector4f 내부에서 sse
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

#ifdef _DEBUG
		Matrix4f& operator*=(const Matrix4f& rhs) noexcept;
		Matrix4f operator*(const Matrix4f& rhs) const noexcept;
#else
		Matrix4f& operator*=(const Matrix4f& rhs) noexcept
		{
			ASSERT(false, "Matrix4f::operator*= is not implemented yet.");

			///SSE version, dp_ps를 사용하지 않음. 5950x 기준으로 아래 버젼이 1.5배정도 더 빠름.
			/*Matrix4f temp = *this;
			Matrix4f transpose = rhs.Transpose();

			const __m128 row0 = _mm_load_ps(transpose.e[0]);
			const __m128 row1 = _mm_load_ps(transpose.e[1]);
			const __m128 row2 = _mm_load_ps(transpose.e[2]);
			const __m128 row3 = _mm_load_ps(transpose.e[3]);

			__m128 col = _mm_load_ps(temp.e[0]);
			__m128 X = _mm_mul_ps(col, row0);
			__m128 Y = _mm_mul_ps(col, row1);
			__m128 Z = _mm_mul_ps(col, row2);
			__m128 W = _mm_mul_ps(col, row3);
			__m128 R = _mm_hadd_ps(_mm_hadd_ps(X, Y), _mm_hadd_ps(Z, W));
			_mm_store_ps(this->e[0], R);

			col = _mm_load_ps(temp.e[1]);
			X = _mm_mul_ps(col, row0);
			Y = _mm_mul_ps(col, row1);
			Z = _mm_mul_ps(col, row2);
			W = _mm_mul_ps(col, row3);
			R = _mm_hadd_ps(_mm_hadd_ps(X, Y), _mm_hadd_ps(Z, W));
			_mm_store_ps(this->e[1], R);

			col = _mm_load_ps(temp.e[2]);
			X = _mm_mul_ps(col, row0);
			Y = _mm_mul_ps(col, row1);
			Z = _mm_mul_ps(col, row2);
			W = _mm_mul_ps(col, row3);
			R = _mm_hadd_ps(_mm_hadd_ps(X, Y), _mm_hadd_ps(Z, W));
			_mm_store_ps(this->e[2], R);

			col = _mm_load_ps(temp.e[3]);
			X = _mm_mul_ps(col, row0);
			Y = _mm_mul_ps(col, row1);
			Z = _mm_mul_ps(col, row2);
			W = _mm_mul_ps(col, row3);
			R = _mm_hadd_ps(_mm_hadd_ps(X, Y), _mm_hadd_ps(Z, W));
			_mm_store_ps(this->e[3], R);

			return *this;*/

			//Mul(rhs.v[0].m, rhs.v[1].m, rhs.v[2].m, rhs.v[3].m);
			//return *this;

			///SSE4.1 dp_ps version
			Matrix4f temp = *this;
			Vector4f transposeRhs[4] =
			{
				{rhs.e[0][0], rhs.e[1][0], rhs.e[2][0], rhs.e[3][0]},
				{rhs.e[0][1], rhs.e[1][1], rhs.e[2][1], rhs.e[3][1]},
				{rhs.e[0][2], rhs.e[1][2], rhs.e[2][2], rhs.e[3][2]},
				{rhs.e[0][3], rhs.e[1][3], rhs.e[2][3], rhs.e[3][3]}
			};

			e[0][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, transposeRhs[0].m, 0xFF));
			e[0][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, transposeRhs[1].m, 0xFF));
			e[0][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, transposeRhs[2].m, 0xFF));
			e[0][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[0].m, transposeRhs[3].m, 0xFF));

			e[1][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, transposeRhs[0].m, 0xFF));
			e[1][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, transposeRhs[1].m, 0xFF));
			e[1][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, transposeRhs[2].m, 0xFF));
			e[1][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[1].m, transposeRhs[3].m, 0xFF));

			e[2][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, transposeRhs[0].m, 0xFF));
			e[2][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, transposeRhs[1].m, 0xFF));
			e[2][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, transposeRhs[2].m, 0xFF));
			e[2][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[2].m, transposeRhs[3].m, 0xFF));

			e[3][0] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, transposeRhs[0].m, 0xFF));
			e[3][1] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, transposeRhs[1].m, 0xFF));
			e[3][2] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, transposeRhs[2].m, 0xFF));
			e[3][3] = _mm_cvtss_f32(_mm_dp_ps(temp.v[3].m, transposeRhs[3].m, 0xFF));

			return *this;

			///DX 버젼과 비슷한 버젼
			/*Matrix4f temp = *this;

			__m128 xxxx = _mm_broadcast_ss(temp.e[0] + 0);
			__m128 yyyy = _mm_broadcast_ss(temp.e[0] + 1);
			__m128 zzzz = _mm_broadcast_ss(temp.e[0] + 2);
			__m128 wwww = _mm_broadcast_ss(temp.e[0] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			this->v[0] = xxxx;

			xxxx = _mm_broadcast_ss(temp.e[1] + 0);
			yyyy = _mm_broadcast_ss(temp.e[1] + 1);
			zzzz = _mm_broadcast_ss(temp.e[1] + 2);
			wwww = _mm_broadcast_ss(temp.e[1] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			this->v[1] = xxxx;

			xxxx = _mm_broadcast_ss(temp.e[2] + 0);
			yyyy = _mm_broadcast_ss(temp.e[2] + 1);
			zzzz = _mm_broadcast_ss(temp.e[2] + 2);
			wwww = _mm_broadcast_ss(temp.e[2] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			this->v[2] = xxxx;

			xxxx = _mm_broadcast_ss(temp.e[3] + 0);
			yyyy = _mm_broadcast_ss(temp.e[3] + 1);
			zzzz = _mm_broadcast_ss(temp.e[3] + 2);
			wwww = _mm_broadcast_ss(temp.e[3] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			this->v[3] = xxxx;

			return *this;*/
		}
		Matrix4f operator*(const Matrix4f& rhs) const noexcept
		{
			Matrix4f result;
			__m128 xxxx = _mm_broadcast_ss(this->e[0] + 0);
			__m128 yyyy = _mm_broadcast_ss(this->e[0] + 1);
			__m128 zzzz = _mm_broadcast_ss(this->e[0] + 2);
			__m128 wwww = _mm_broadcast_ss(this->e[0] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			result.v[0] = xxxx;

			xxxx = _mm_broadcast_ss(this->e[1] + 0);
			yyyy = _mm_broadcast_ss(this->e[1] + 1);
			zzzz = _mm_broadcast_ss(this->e[1] + 2);
			wwww = _mm_broadcast_ss(this->e[1] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			result.v[1] = xxxx;

			xxxx = _mm_broadcast_ss(this->e[2] + 0);
			yyyy = _mm_broadcast_ss(this->e[2] + 1);
			zzzz = _mm_broadcast_ss(this->e[2] + 2);
			wwww = _mm_broadcast_ss(this->e[2] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			result.v[2] = xxxx;

			xxxx = _mm_broadcast_ss(this->e[3] + 0);
			yyyy = _mm_broadcast_ss(this->e[3] + 1);
			zzzz = _mm_broadcast_ss(this->e[3] + 2);
			wwww = _mm_broadcast_ss(this->e[3] + 3);
			xxxx = _mm_mul_ps(xxxx, rhs.v[0].m);
			yyyy = _mm_mul_ps(yyyy, rhs.v[1].m);
			zzzz = _mm_mul_ps(zzzz, rhs.v[2].m);
			wwww = _mm_mul_ps(wwww, rhs.v[3].m);
			xxxx = _mm_add_ps(xxxx, zzzz);
			yyyy = _mm_add_ps(yyyy, wwww);
			xxxx = _mm_add_ps(xxxx, yyyy);
			result.v[3] = xxxx;

			return result;
		}
#endif

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

		static Matrix4f Zero() noexcept
		{
			Matrix4f temp
			{
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
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

		void Mul(__m128 rhsR0, __m128 rhsR1, __m128 rhsR2, __m128 rhsR3)
		{
			Matrix4f temp = *this;
			__m128 vW = temp.v[0].m;
			// Splat the component X,Y,Z then W
			__m128 vX = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(0, 0, 0, 0));
			__m128 vY = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(1, 1, 1, 1));
			__m128 vZ = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(3, 3, 3, 3));
			// Perform the operation on the first row
			vX = _mm_mul_ps(vX, rhsR0);
			vY = _mm_mul_ps(vY, rhsR1);
			vZ = _mm_mul_ps(vZ, rhsR2);
			vW = _mm_mul_ps(vW, rhsR3);
			// Perform a binary add to reduce cumulative errors
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			this->v[0] = vX;
			// Repeat for the other 3 rows
			vW = temp.v[1].m;
			vX = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(0, 0, 0, 0));
			vY = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(1, 1, 1, 1));
			vZ = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(3, 3, 3, 3));
			vX = _mm_mul_ps(vX, rhsR0);
			vY = _mm_mul_ps(vY, rhsR1);
			vZ = _mm_mul_ps(vZ, rhsR2);
			vW = _mm_mul_ps(vW, rhsR3);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			this->v[1] = vX;
			vW = temp.v[2].m;
			vX = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(0, 0, 0, 0));
			vY = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(1, 1, 1, 1));
			vZ = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(3, 3, 3, 3));
			vX = _mm_mul_ps(vX, rhsR0);
			vY = _mm_mul_ps(vY, rhsR1);
			vZ = _mm_mul_ps(vZ, rhsR2);
			vW = _mm_mul_ps(vW, rhsR3);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			this->v[2] = vX;
			vW = temp.v[3].m;
			vX = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(0, 0, 0, 0));
			vY = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(1, 1, 1, 1));
			vZ = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(2, 2, 2, 2));
			vW = _mm_shuffle_ps((vW), (vW), _MM_SHUFFLE(3, 3, 3, 3));
			vX = _mm_mul_ps(vX, rhsR0);
			vY = _mm_mul_ps(vY, rhsR1);
			vZ = _mm_mul_ps(vZ, rhsR2);
			vW = _mm_mul_ps(vW, rhsR3);
			vX = _mm_add_ps(vX, vZ);
			vY = _mm_add_ps(vY, vW);
			vX = _mm_add_ps(vX, vY);
			this->v[3] = vX;
		}

		union
		{
			Vector4f v[4];
			float e[4][4];
		};
	};

#ifdef _DEBUG
	FLOATER_MATH_API void Matrix4fMul(const Matrix4f& lhs, Matrix4f& out, __m128 rhsR0, __m128 rhsR1, __m128 rhsR2, __m128 rhsR3);

	[[deprecated("use flt::Matrix4fMul function")]]
	FLOATER_MATH_API void Matrix4fMuluseDot(const Matrix4f& lhs, const Matrix4f& rhs, Matrix4f& out);

#else
	__forceinline void Matrix4fMul(const Matrix4f& lhs, Matrix4f& out, __m128 rhsR0, __m128 rhsR1, __m128 rhsR2, __m128 rhsR3)
	{
		__m128 xxxx = _mm_broadcast_ss(lhs.e[0] + 0);
		__m128 yyyy = _mm_broadcast_ss(lhs.e[0] + 1);
		__m128 zzzz = _mm_broadcast_ss(lhs.e[0] + 2);
		__m128 wwww = _mm_broadcast_ss(lhs.e[0] + 3);
		xxxx = _mm_mul_ps(xxxx, rhsR0);
		yyyy = _mm_mul_ps(yyyy, rhsR1);
		zzzz = _mm_mul_ps(zzzz, rhsR2);
		wwww = _mm_mul_ps(wwww, rhsR3);
		xxxx = _mm_add_ps(xxxx, zzzz);
		yyyy = _mm_add_ps(yyyy, wwww);
		xxxx = _mm_add_ps(xxxx, yyyy);
		out.v[0] = xxxx;

		xxxx = _mm_broadcast_ss(lhs.e[1] + 0);
		yyyy = _mm_broadcast_ss(lhs.e[1] + 1);
		zzzz = _mm_broadcast_ss(lhs.e[1] + 2);
		wwww = _mm_broadcast_ss(lhs.e[1] + 3);
		xxxx = _mm_mul_ps(xxxx, rhsR0);
		yyyy = _mm_mul_ps(yyyy, rhsR1);
		zzzz = _mm_mul_ps(zzzz, rhsR2);
		wwww = _mm_mul_ps(wwww, rhsR3);
		xxxx = _mm_add_ps(xxxx, zzzz);
		yyyy = _mm_add_ps(yyyy, wwww);
		xxxx = _mm_add_ps(xxxx, yyyy);
		out.v[1] = xxxx;

		xxxx = _mm_broadcast_ss(lhs.e[2] + 0);
		yyyy = _mm_broadcast_ss(lhs.e[2] + 1);
		zzzz = _mm_broadcast_ss(lhs.e[2] + 2);
		wwww = _mm_broadcast_ss(lhs.e[2] + 3);
		xxxx = _mm_mul_ps(xxxx, rhsR0);
		yyyy = _mm_mul_ps(yyyy, rhsR1);
		zzzz = _mm_mul_ps(zzzz, rhsR2);
		wwww = _mm_mul_ps(wwww, rhsR3);
		xxxx = _mm_add_ps(xxxx, zzzz);
		yyyy = _mm_add_ps(yyyy, wwww);
		xxxx = _mm_add_ps(xxxx, yyyy);
		out.v[2] = xxxx;

		xxxx = _mm_broadcast_ss(lhs.e[3] + 0);
		yyyy = _mm_broadcast_ss(lhs.e[3] + 1);
		zzzz = _mm_broadcast_ss(lhs.e[3] + 2);
		wwww = _mm_broadcast_ss(lhs.e[3] + 3);
		xxxx = _mm_mul_ps(xxxx, rhsR0);
		yyyy = _mm_mul_ps(yyyy, rhsR1);
		zzzz = _mm_mul_ps(zzzz, rhsR2);
		wwww = _mm_mul_ps(wwww, rhsR3);
		xxxx = _mm_add_ps(xxxx, zzzz);
		yyyy = _mm_add_ps(yyyy, wwww);
		xxxx = _mm_add_ps(xxxx, yyyy);
		out.v[3] = xxxx;
	}

	// _mm_dp_ps를 사용한 버젼은 Matrix4fMul보다 Release에서 4배정도 느리다.
	// 동일 연산 비교 시
	// Matrix4fMul : 1622ms
	// Matrix4fMulDot : 6406ms
	[[deprecated("use flt::Matrix4fMul function")]]
	__forceinline void Matrix4fMuluseDot(const Matrix4f& lhs, const Matrix4f& rhs, Matrix4f& out)
	{
		Vector4f transposeRhs[4] =
		{
			{rhs.e[0][0], rhs.e[1][0], rhs.e[2][0], rhs.e[3][0]},
			{rhs.e[0][1], rhs.e[1][1], rhs.e[2][1], rhs.e[3][1]},
			{rhs.e[0][2], rhs.e[1][2], rhs.e[2][2], rhs.e[3][2]},
			{rhs.e[0][3], rhs.e[1][3], rhs.e[2][3], rhs.e[3][3]}
		};

		out.e[0][0] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[0].m, transposeRhs[0].m, 0xFF));
		out.e[0][1] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[0].m, transposeRhs[1].m, 0xFF));
		out.e[0][2] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[0].m, transposeRhs[2].m, 0xFF));
		out.e[0][3] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[0].m, transposeRhs[3].m, 0xFF));

		out.e[1][0] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[1].m, transposeRhs[0].m, 0xFF));
		out.e[1][1] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[1].m, transposeRhs[1].m, 0xFF));
		out.e[1][2] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[1].m, transposeRhs[2].m, 0xFF));
		out.e[1][3] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[1].m, transposeRhs[3].m, 0xFF));

		out.e[2][0] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[2].m, transposeRhs[0].m, 0xFF));
		out.e[2][1] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[2].m, transposeRhs[1].m, 0xFF));
		out.e[2][2] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[2].m, transposeRhs[2].m, 0xFF));
		out.e[2][3] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[2].m, transposeRhs[3].m, 0xFF));

		out.e[3][0] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[3].m, transposeRhs[0].m, 0xFF));
		out.e[3][1] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[3].m, transposeRhs[1].m, 0xFF));
		out.e[3][2] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[3].m, transposeRhs[2].m, 0xFF));
		out.e[3][3] = _mm_cvtss_f32(_mm_dp_ps(lhs.v[3].m, transposeRhs[3].m, 0xFF));
	}
#endif
	// 행렬 곱셈
	
}

