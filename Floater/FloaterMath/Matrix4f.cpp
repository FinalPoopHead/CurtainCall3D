#include "./include/Matrix4f.h"


namespace flt
{
#ifdef _DEBUG
	Matrix4f& Matrix4f::operator*=(const Matrix4f& rhs) noexcept
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

	Matrix4f Matrix4f::operator*(const Matrix4f& rhs) const noexcept
	{
		Matrix4f result;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.e[i][j] = e[i][0] * rhs.e[0][j] + e[i][1] * rhs.e[1][j] + e[i][2] * rhs.e[2][j] + e[i][3] * rhs.e[3][j];
			}
		}

		return result;

		/*Matrix4f result;
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

		return result;*/
	}

	void Matrix4fMul(const Matrix4f& lhs, Matrix4f& out, __m128 rhsR0, __m128 rhsR1, __m128 rhsR2, __m128 rhsR3)
	{
		for (int i = 0; i < 4; ++i)
		{
			out.e[i][0] = lhs.e[i][0] * rhsR0.m128_f32[0] + lhs.e[i][1] * rhsR1.m128_f32[0] + lhs.e[i][2] * rhsR2.m128_f32[0] + lhs.e[i][3] * rhsR3.m128_f32[0];
			out.e[i][1] = lhs.e[i][0] * rhsR0.m128_f32[1] + lhs.e[i][1] * rhsR1.m128_f32[1] + lhs.e[i][2] * rhsR2.m128_f32[1] + lhs.e[i][3] * rhsR3.m128_f32[1];
			out.e[i][2] = lhs.e[i][0] * rhsR0.m128_f32[2] + lhs.e[i][1] * rhsR1.m128_f32[2] + lhs.e[i][2] * rhsR2.m128_f32[2] + lhs.e[i][3] * rhsR3.m128_f32[2];
			out.e[i][3] = lhs.e[i][0] * rhsR0.m128_f32[3] + lhs.e[i][1] * rhsR1.m128_f32[3] + lhs.e[i][2] * rhsR2.m128_f32[3] + lhs.e[i][3] * rhsR3.m128_f32[3];
		}


		/*__m128 xxxx = _mm_broadcast_ss(lhs.e[0] + 0);
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
		out.v[3] = xxxx;*/
	}

	void Matrix4fMuluseDot(const Matrix4f& lhs, const Matrix4f& rhs, Matrix4f& out)
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
	///DX 의존성 제거를 위한 주석처리
	//Matrix4f::operator DirectX::XMMATRIX() const noexcept
	//{
	//	return DirectX::XMMATRIX
	//	{
	//		e[0][0], e[0][1], e[0][2], e[0][3],
	//			e[1][0], e[1][1], e[1][2], e[1][3],
	//			e[2][0], e[2][1], e[2][2], e[2][3],
	//			e[3][0], e[3][1], e[3][2], e[3][3],
	//	};
	//}

	/*void Matrix4fMul(const Matrix4f& lhs, Matrix4f& out, __m128 rhsR0, __m128 rhsR1, __m128 rhsR2, __m128 rhsR3)
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
	}*/



}
