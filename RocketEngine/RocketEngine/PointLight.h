#pragma once
#include "DLLExporter.h"
#include "Light.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket
{
	class ROCKET_API PointLight final : public Light
	{
	public:
		PointLight();

	public:
		//Rocket::Core::PointLightData GetLightData() const;

	public:
		float GetRange() const;
		void SetRange(float range);
		Vector3 GetAttenuation() const;
		void SetAttenuation(Vector3 attenuation);

	private:
		float _range;
		Vector3 _attenuation;
	};
}
