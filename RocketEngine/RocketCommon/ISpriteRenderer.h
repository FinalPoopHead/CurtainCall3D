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
	};
}
