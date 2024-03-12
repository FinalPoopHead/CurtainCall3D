#pragma once
#include <string>

#include "MathHeader.h"

namespace Rocket::Core
{
	class CubeMap
	{
	public:
		CubeMap();
		~CubeMap();

		void SetTexture(std::wstring filePath);
	};
}

