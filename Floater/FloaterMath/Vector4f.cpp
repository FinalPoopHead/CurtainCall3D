
#include "./include/Vector4f.h"
#include "./include/Matrix4f.h"

namespace flt
{
	Vector4f::Vector4f(const Vector3f& v, float w) noexcept : m{ .m128_f32{v.x, v.y, v.z, w} }
	{

	}

	Vector4f::operator Vector3f() const noexcept
	{
		return Vector3f(m.m128_f32[0], m.m128_f32[1], m.m128_f32[2]);
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

		return *this;
	}
}
