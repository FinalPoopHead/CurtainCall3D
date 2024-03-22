#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	struct PrimitiveBase
	{
		Matrix worldTM;
		bool isWire;
		Color color;
	};

	struct CubePrimitive : public PrimitiveBase
	{
		Vector3 widthHeightDepth;
	};

	struct SpherePrimitive : public PrimitiveBase
	{
		float diameter;
	};

	struct CylinderPrimitive : public PrimitiveBase
	{
		float height;
		float diameter;
	};
}
