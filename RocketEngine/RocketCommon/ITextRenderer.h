#pragma once
#include <windows.h>
#include <string>
#include "MathHeader.h"
#include "IGraphicsObject.h"

namespace Rocket::Core
{
	class ITextRenderer : public IGraphicsObject
	{
	public:
		virtual std::string& GetText() = 0;
		virtual Color GetColor() = 0;
		virtual void SetText(const std::string& str) = 0;
		virtual void SetColor(Color color) = 0;
	};
}
