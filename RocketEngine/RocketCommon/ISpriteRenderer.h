#pragma once
#include <string>
#include "IGraphicsObject.h"

namespace Rocket::Core
{
	class ISpriteRenderer : public IGraphicsObject
	{
	public:
		virtual void SetImage(const std::string& filePath) = 0;
		virtual void SetColor(Color color) = 0;
		virtual float GetWidth() = 0;
		virtual float GetHeight() = 0;
	};
}
