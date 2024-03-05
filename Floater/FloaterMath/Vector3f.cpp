#include "../FloaterUtil/include/FloaterMacro.h"
#include "./include/Common.h"
#include "./include/Matrix3f.h"
#include "./include/Vector3f.h"

bool flt::Vector3f::operator==(const Vector3f& rhs) const noexcept
{
	float eps = FLOAT_EPSILON * fmaxf(fabsf(x), fabsf(rhs.x));
	bool ret = fabsf(x - rhs.x) <= eps;

	eps = FLOAT_EPSILON * fmaxf(fabsf(y), fabsf(rhs.y));
	ret &= fabsf(y - rhs.y) <= eps;

	eps = FLOAT_EPSILON * fmaxf(fabsf(z), fabsf(rhs.z));
	ret &= fabsf(z - rhs.z) <= eps;

	return ret;
}

constexpr flt::Vector3f& flt::Vector3f::operator*=(const Matrix3f& rhs) noexcept
{
	Vector3f tmp = *this;
	x = tmp.x * rhs.e[0][0] + tmp.y * rhs.e[1][0] + tmp.z * rhs.e[2][0];
	y = tmp.x * rhs.e[0][1] + tmp.y * rhs.e[1][1] + tmp.z * rhs.e[2][1];
	z = tmp.x * rhs.e[0][2] + tmp.y * rhs.e[1][2] + tmp.z * rhs.e[2][2];

	return *this;
}

flt::Vector3f flt::Vector3f::Lerp(const Vector3f& v0, const Vector3f& v1, float t) noexcept
{
	ASSERT(t >= 0.0f && t <= 1.0f, "");
	return v0 + (v1 - v0) * t;
}
