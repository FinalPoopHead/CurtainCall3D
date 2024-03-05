#include "DLLExporter.h"
#include "Light.h"
#include "GraphicsSystem.h"

namespace Rocket
{
	Light::Light()
		: _ambient(1.0f,1.0f,1.0f,1.0f),
		_diffuse(1.0f,1.0f,1.0f,1.0f),
		_specular(1.0f,1.0f,1.0f,1.0f)
	{
		//Rocket::Core::RenderSystem::Instance().AddLight(this);
	}

	Vector4 Light::GetAmbient() const
	{
		return _ambient;
	}

	void Light::SetAmbient(Vector4 ambient)
	{
		_ambient = ambient;
	}

	Vector4 Light::GetDiffuse() const
	{
		return _diffuse;
	}

	void Light::SetDiffuse(Vector4 diffuse)
	{
		_diffuse = diffuse;
	}

	Vector4 Light::GetSpecular() const
	{
		return _specular;
	}

	void Light::SetSpecular(Vector4 specular)
	{
		_specular = specular;
	}

}
