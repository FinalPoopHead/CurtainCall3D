
#include "./include/Vector4f.h"
#include "./include/Matrix4f.h"

namespace flt
{
	//Vector4f::Vector4f(const Vector3f& v, float w) noexcept : m{ .m128_f32{v.x, v.y, v.z, w} }
	//{

	//}

	Vector4f Vector4f::Lerp(const Vector4f& a, const Vector4f& b, float t) noexcept
	{
		return a + (b - a) * t;
	}

	//Vector4f::operator Vector3f() const noexcept
	//{
	//	return Vector3f(m.m128_f32[0], m.m128_f32[1], m.m128_f32[2]);
	//}

	Vector4f::Vector4f(const Vector3f& v, float w) noexcept : e{v.x, v.y, v.z, w}
	{

	}

	Vector4f::operator Vector3f() const noexcept
	{
		return Vector3f(e[0], e[1], e[2]);
	}

	Vector4f::operator __m128() const noexcept
	{
		return m;
	}

	Vector4f& Vector4f::operator*=(const Matrix4f& rhs) noexcept
	{
		Vector4f tmp = *this;

		x = tmp.x * rhs.e[0][0] + tmp.y * rhs.e[1][0] + tmp.z * rhs.e[2][0] + tmp.w * rhs.e[3][0];
		y = tmp.x * rhs.e[0][1] + tmp.y * rhs.e[1][1] + tmp.z * rhs.e[2][1] + tmp.w * rhs.e[3][1];
		z = tmp.x * rhs.e[0][2] + tmp.y * rhs.e[1][2] + tmp.z * rhs.e[2][2] + tmp.w * rhs.e[3][2];
		w = tmp.x * rhs.e[0][3] + tmp.y * rhs.e[1][3] + tmp.z * rhs.e[2][3] + tmp.w * rhs.e[3][3];

		//__m128 x = _mm_set1_ps(tmp.x);
		//__m128 y = _mm_set1_ps(tmp.y);
		//__m128 z = _mm_set1_ps(tmp.z);
		//__m128 w = _mm_set1_ps(tmp.w);

		//__m128 p0 = _mm_mul_ps(x, rhs.v[0].m);
		//__m128 p1 = _mm_mul_ps(y, rhs.v[1].m);
		//__m128 p2 = _mm_mul_ps(z, rhs.v[2].m);
		//__m128 p3 = _mm_mul_ps(w, rhs.v[3].m);

		//this->m = _mm_add_ps(_mm_add_ps(p0, p1), _mm_add_ps(p2, p3));

		return *this;
	}
}
