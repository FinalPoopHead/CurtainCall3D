#pragma once
#include <DirectXCollision.h>
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

		// For Shadow Map
	public:
		void Update();
		bool FrustumCulling(const DirectX::BoundingBox& boundingBox);
		bool FrustumCulling(const DirectX::BoundingOrientedBox& boundingOrientedBox);
		bool FrustumCulling(const DirectX::BoundingSphere& boundingSphere);

		Matrix GetViewMatrix() const { return _viewMatrix; }	
		Matrix GetProjectionMatrix() const { return _projectionMatrix; }


	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();

	private:
		RocketTransform* _transform;

		// Legacy Light Properties
		Color _diffuseColor;
		Color _ambientColor;
		Color _specularColor;
		float _specularPower;

		float _lengthZ;		// TODO : 매번 업데이트되는데 여기다 둘까? 흠..
		float _shadowRadius;

		// For Shadow Map.
	private:
		DirectX::BoundingFrustum _boundingFrustum;
		Vector3 _lightPosForShadow;					// ShadowMap을 찍기위해 적절한 위치로 옮긴 position 값. viewMatrix를 만들기 위해 사용.

		DirectX::XMFLOAT4X4 _viewMatrix;
		DirectX::XMFLOAT4X4 _projectionMatrix;
	};
}
