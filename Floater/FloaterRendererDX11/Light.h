#pragma once
#include <DirectXMath.h>

namespace flt
{
	struct DirectionalLight
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT4 specularColor;

		DirectX::XMFLOAT3 direction;

		float padding;
	};

	struct PointLight
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT4 specularColor;
		DirectX::XMFLOAT3 attenuation;

		float range;
		DirectX::XMFLOAT3 position;

		float padding;
	};

	struct SpotLight
	{
		DirectX::XMFLOAT4 ambientColor;
		DirectX::XMFLOAT4 diffuseColor;
		DirectX::XMFLOAT4 specularColor;
		DirectX::XMFLOAT3 attenuation;

		float range;
		DirectX::XMFLOAT3 position;
		float spot;
		DirectX::XMFLOAT3 direction;

		float padding;
	};
}
