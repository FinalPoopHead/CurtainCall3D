#pragma once
#include <windows.h>
#include <string>
#include "MathHeader.h"
#include "IRenderable.h"

namespace Rocket::Core
{
	class ITextRenderer : public IRenderable
	{
	public:
		virtual std::string& GetText() = 0;
		virtual Color GetColor() = 0;
		virtual void SetText(const std::string& str) = 0;
		virtual void SetColor(Color color) = 0;
	};
}
