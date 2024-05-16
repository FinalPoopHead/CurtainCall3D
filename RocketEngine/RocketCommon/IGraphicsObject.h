#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class IGraphicsObject
	{
	public:
		virtual ~IGraphicsObject() {}
		virtual void SetWorldTM(const Matrix& worldTM) = 0;
		virtual void SetActive(bool isActive) = 0;
	};
}
