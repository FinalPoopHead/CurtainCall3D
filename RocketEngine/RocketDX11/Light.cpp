#include "Light.h"

namespace Rocket::Core
{

	void Light::SetDiffuseColor(float r, float g, float b, float a)
	{
		_diffuseColor.x = r;
		_diffuseColor.y = g;
		_diffuseColor.z = b;
		_diffuseColor.w = a;
	}

	void Light::SetDirection(float x, float y, float z)
	{
		_direction.x = x;
		_direction.y = y;
		_direction.z = z;
	}

	DirectX::XMFLOAT4 Light::GetDiffuseColor()
	{
		return _diffuseColor;
	}

	DirectX::XMFLOAT3 Light::GetDirection()
	{
		return _direction;
	}

}