#pragma once
#include "MathHeader.h"
#include "../RocketCommon/IDirectionalLight.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	class DirectionalLight : public IDirectionalLight
	{
	public:
		DirectionalLight();

		virtual void BindTransform(RocketTransform* transform) override { _transform = transform; }
		virtual void SetDiffuseColor(const Color& color) override { _diffuseColor = color; }
		virtual void SetAmbientColor(const Color& color) override { _ambientColor = color; }
		virtual void SetSpecularColor(const Color& color) override { _specularColor = color; }
		virtual void SetSpecularPower(float power) override { _specularPower = power; }

		const Color& GetDiffuseColor() const { return _diffuseColor; }
		const Color& GetAmbientColor() const { return _ambientColor; }
		const Color& GetSpecularColor() const { return _specularColor; }
		float GetSpecularPower() const { return _specularPower; }
		Vector3 GetForward() const;

	private:
		RocketTransform* _transform;

		// Legacy Light Properties
		Color _diffuseColor;
		Color _ambientColor;
		Color _specularColor;
		float _specularPower;
	};
}
