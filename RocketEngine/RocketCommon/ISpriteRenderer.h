#pragma once
#include <string>
#include "IRenderable.h"

namespace Rocket::Core
{
	class ISpriteRenderer : public IRenderable
	{
	public:
		virtual void SetImage(const std::string& fileName) = 0;
		virtual void SetColor(Color color) = 0;
	};
}
