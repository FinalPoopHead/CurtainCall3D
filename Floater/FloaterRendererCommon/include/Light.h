#pragma once
#include "../../FloaterMath/include/Vector3f.h"


namespace flt
{
	struct Light
	{
	public:
		Light() = default;
		Light(const Vector3f& position, const Vector3f& color, float intensity)
			: position(position), intensity(intensity)
		{
		}

		Vector4f ambientColor;
		Vector4f diffuseColor;
		Vector4f specularColor;
		Vector3f attenuation;
		Vector3f position;
		float range;
		Vector3f direction;
		float intensity;
	};
}
