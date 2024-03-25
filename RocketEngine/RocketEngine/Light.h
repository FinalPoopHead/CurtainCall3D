#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "MathHeader.h"

namespace Rocket::Core
{
	class ILight;
}

namespace Rocket
{
	class ROCKET_API Light : public Component
	{
	public:
		Light();
		virtual ~Light() {}

	public:
		virtual void BindTransform() override;
		Color GetDiffuseColor() const { return _diffuseColor; }
		void SetDiffuseColor(Color diffuse);
		Color GetAmbientColor() const { return _ambientColor; }
		void SetAmbientColor(Color ambient);
		Color GetSpecularColor() const { return _specularColor; }
		void SetSpecularColor(Color specular);
		float GetSpecularPower() const { return _specularPower; }
		void SetSpecularPower(float power);

	protected:
		Core::ILight* _light;
		Color _diffuseColor;
		Color _ambientColor;
		Color _specularColor;
		float _specularPower;
	};
}
