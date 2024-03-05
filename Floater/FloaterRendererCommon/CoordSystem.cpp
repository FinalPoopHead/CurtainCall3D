#include "CoordSystem.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterMath/include/Matrix3f.h"
#include <iostream>

flt::CoordSystem::Axis flt::CoordSystem::ConvAxis(const Vector3f& vec)
{
	if (vec == ConvVec(Axis::X) || vec == -ConvVec(Axis::X))
	{
		return Axis::X;
	}
	else if (vec == ConvVec(Axis::Y) || vec == -ConvVec(Axis::Y))
	{
		return Axis::Y;
	}
	else if (vec == ConvVec(Axis::Z) || vec == -ConvVec(Axis::Z))
	{
		return Axis::Z;
	}
	else
	{
		ASSERT(false, "The vector is not a unit vector.");
		return Axis::NONE;
	}
}

flt::Vector3f flt::CoordSystem::ConvVec(Axis axis)
{
	switch (axis)
	{
		case flt::CoordSystem::Axis::X:
		{
			return Vector3f{ 1.f, 0.f, 0.f };
		}
		break;
		case flt::CoordSystem::Axis::Y:
		{
			return Vector3f{ 0.f, 1.f, 0.f };
		}
		break;
		case flt::CoordSystem::Axis::Z:
		{
			return Vector3f{ 0.f, 0.f, 1.f };
		}
		break;
		default:
		{
			ASSERT(false, "The axis is NONE.");
			return Vector3f::Zero();
		}
		break;
	}
}

// static member

char flt::CoordSystem::AxisNameTable[3] = { 'X', 'Y', 'Z' };

flt::CoordSystem flt::CoordSystem::DX11()
{
	return CoordSystem(1, 1, Axis::Y, Axis::Z, false);
}

flt::CoordSystem flt::CoordSystem::OpenGL()
{
	return CoordSystem(1, -1, Axis::Y, Axis::Z, true);
}

flt::CoordSystem flt::CoordSystem::Unity()
{
	return DX11();
}

flt::CoordSystem flt::CoordSystem::Unreal()
{
	return CoordSystem(1, 1, Axis::Z, Axis::X, false);
}

flt::CoordSystem flt::CoordSystem::Max3D()
{
	return CoordSystem(1, 1, Axis::Z, Axis::Y, true);
}

void flt::CoordSystem::Convert(const CoordSystem& src, const CoordSystem& dest, Vector3f& outConvData)
{
	Matrix3f mat = Matrix3f::Identity();

	int srcUpIndex = static_cast<int>(src._up) - 1;
	int srcFrontIndex = static_cast<int>(src._front) - 1;
	int srcRightIndex = 3 - srcUpIndex - srcFrontIndex;

	float srcUpSign = src._upVec.x + src._upVec.y + src._upVec.z;
	float srcFrontSign = src._frontVec.x + src._frontVec.y + src._frontVec.z;
	float srcRightSign = src._rightVec.x + src._rightVec.y + src._rightVec.z;

	mat.v[srcUpIndex] = dest._upVec * srcUpSign;
	mat.v[srcFrontIndex] = dest._frontVec * srcFrontSign;
	mat.v[srcRightIndex] = dest._rightVec * srcRightSign;

	outConvData = outConvData * mat;
}

// member

flt::CoordSystem::CoordSystem() : CoordSystem(1, 1, Axis::Y, Axis::Z, true)
{

}

flt::CoordSystem::CoordSystem(int upSign, int frontSign, Axis up, Axis front, bool rightHanded) :
	CoordSystem{ ConvVec(up) * (float)upSign, ConvVec(front) * (float)frontSign, rightHanded }
{

}

flt::CoordSystem::CoordSystem(const Vector3f& up, const Vector3f& front, bool rightHanded) :
	_upVec(up), _frontVec(front), _rightVec(up.Cross(front) * (!rightHanded ? 1.f : -1.f)),
	_rightHanded(rightHanded),
	_up(ConvAxis(up)), _front(ConvAxis(front)), _right(ConvAxis(up.Cross(front)))
{
}

void flt::CoordSystem::ConvertFrom(CoordSystem srcCoord, Vector3f& outConvData)
{
	Matrix3f mat = Matrix3f::Identity();

	int srcUpindex = static_cast<int>(srcCoord._up) - 1;
	int srcFrontIndex = static_cast<int>(srcCoord._front) - 1;
	int srcRightIndex = 3 - srcUpindex - srcFrontIndex;

	float srcUpSign = srcCoord._upVec.x + srcCoord._upVec.y + srcCoord._upVec.z;
	float srcFrontSign = srcCoord._frontVec.x + srcCoord._frontVec.y + srcCoord._frontVec.z;
	float srcRightSign = srcCoord._rightVec.x + srcCoord._rightVec.y + srcCoord._rightVec.z;

	mat.v[srcUpindex] = _upVec * srcUpSign;
	mat.v[srcFrontIndex] = _frontVec * srcFrontSign;
	mat.v[srcRightIndex] = _rightVec * srcRightSign;

	outConvData = outConvData * mat;
}

void flt::CoordSystem::PrintfCoord()
{
	std::cout << "Up : " << AxisNameTable[(int)_up - 1]
		<< " Front: " << AxisNameTable[(int)_front - 1]
		<< " Right: " << AxisNameTable[(int)_right - 1] << std::endl;

	std::cout << "Up : " << (int)(_upVec.x + _upVec.y + _upVec.z)
		<< " Front: " << (int)(_frontVec.x + _frontVec.y + _frontVec.z)
		<< " Right: " << (int)(_rightVec.x + _rightVec.y + _rightVec.z) << std::endl;
}

