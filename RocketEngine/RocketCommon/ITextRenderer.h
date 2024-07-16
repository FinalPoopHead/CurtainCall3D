#pragma once
#include <windows.h>
#include <string>
#include "MathHeader.h"
#include "IGraphicsObject.h"

namespace Rocket::Core
{
	enum class eTextAlignment
	{
		LEFT
		, CENTER
		, RIGHT
	};

	class ITextRenderer : public IGraphicsObject
	{
	public:
		virtual std::string& GetText() = 0;
		virtual Color GetColor() = 0;
		virtual void SetFontwithPath(const std::string& fontPath) = 0;
		virtual void SetText(const std::string& str) = 0;
		virtual void SetColor(Color color) = 0;
		virtual void SetTargetCameraIndex(int cameraIndex) = 0;
		virtual void SetTextAlignment(eTextAlignment alignment) = 0;
	};
}
