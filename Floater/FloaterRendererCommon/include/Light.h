#pragma once
#include "../../FloaterMath/include/Vector3f.h"


namespace flt
{
	struct Light
	{
	public:
		enum class Type
		{
			none,
			directional,
			point,
			spot
		};

	public:
		Light() = default;
		Light(const Vector3f& position, const Vector3f& color, float intensity)
			: type(Type::none)
			, ambientColor()
			, diffuseColor()
			, specularColor(color, 1.0f)
			, attenuation()
			, position(position)
			, range(0.0f)
			, direction()
			, intensity(intensity)
		{
		}

		Type type;
		Vector4f ambientColor;
		Vector4f diffuseColor;
		Vector4f specularColor;
		Vector3f attenuation;
		Vector3f position;
		float range;
		Quaternion direction;
		float intensity;
	};
}
