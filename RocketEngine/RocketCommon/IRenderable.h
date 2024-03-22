#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class IRenderable
	{
	public:
		virtual ~IRenderable() {}
		virtual void SetWorldTM(const Matrix& worldTM) = 0;
		virtual void SetActive(bool isActive) = 0;
	};
}