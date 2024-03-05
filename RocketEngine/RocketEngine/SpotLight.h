#pragma once
#include "DLLExporter.h"
#include "Light.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket
{
	class ROCKET_API SpotLight final : public Light
	{
	public:
		SpotLight();

	public:
		//Rocket::Core::SpotLightData GetLightData() const;

	public:
		float GetRange() const;
		void SetRange(float range);
		float GetSpot() const;
		void SetSpot(float spot);
		Vector3 GetAttenuation() const;
		void SetAttenuation(Vector3 attenuation);

	private:
		float _range;
		float _spot;
		Vector3 _attenuation;
	};
}
