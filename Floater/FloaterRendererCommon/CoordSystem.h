#pragma once
#include "../FloaterMath/include/Vector3f.h"

namespace flt
{

	class CoordSystem
	{
	public:
		enum class Axis
		{
			NONE, X, Y, Z
		};

		CoordSystem();
		CoordSystem(int upSign, int frontSign, Axis up, Axis front, bool rightHanded);
		CoordSystem(const Vector3f& up, const Vector3f& front, bool rightHanded);
		//CoordSystem(const Vector3f& up, const Vector3f& front, const Vector3f& right);

		void ConvertFrom(CoordSystem srcCoord, Vector3f& outConvData);

		void PrintfCoord();

		static CoordSystem DX11();
		static CoordSystem OpenGL();
		static CoordSystem Unity();
		static CoordSystem Unreal();
		static CoordSystem Max3D();
		static void Convert(const CoordSystem& src, const CoordSystem& dest, Vector3f& outConvData);

	private:
		Axis ConvAxis(const Vector3f& vec);
		Vector3f ConvVec(Axis Axis);

	private:
		static char AxisNameTable[3];

		Axis _up;
		Axis _front;
		Axis _right;
		Vector3f _upVec;
		Vector3f _frontVec;
		Vector3f _rightVec;
		bool _rightHanded;
	};
}


