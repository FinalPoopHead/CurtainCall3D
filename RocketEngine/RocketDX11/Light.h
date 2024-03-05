#pragma once
#include <SimpleMath.h>

namespace Rocket::Core
{
	class Light
	{
	public:
		void SetDiffuseColor(float r, float g, float b, float a);
		void SetDirection(float x, float y, float z);

		DirectX::XMFLOAT4 GetDiffuseColor();
		DirectX::XMFLOAT3 GetDirection();

	private:
		DirectX::XMFLOAT4 _diffuseColor;
		DirectX::XMFLOAT3 _direction;
	};
}
