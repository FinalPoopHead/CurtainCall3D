#pragma once
#include "MathHeader.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	/// <summary>
	/// Light들의 기본 인터페이스
	/// RocketTransform을 포인터로 참조하고있으므로 그걸 이용해서 위치와 방향을 선택한다.
	/// </summary>
	class ILight
	{
	public:
		virtual ~ILight() {}
		virtual void BindTransform(RocketTransform* transform) = 0;

		virtual void SetDiffuseColor(const Color& color) = 0;
		virtual void SetAmbientColor(const Color& color) = 0;
		virtual void SetSpecularColor(const Color& color) = 0;
		virtual void SetSpecularPower(float power) = 0;
	};
}
