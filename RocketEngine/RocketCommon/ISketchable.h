#pragma once
#include "IRenderable.h"

namespace Rocket::Core
{
	class ISketchable : public IRenderable
	{
	public:
		virtual ~ISketchable() {};
		virtual void SetScreenSpace() = 0;
		virtual void SetWorldSpace() = 0;

		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;
	};
}