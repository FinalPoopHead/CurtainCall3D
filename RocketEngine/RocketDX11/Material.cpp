#include "Material.h"

namespace Rocket::Core
{
	Material::Material()
		: _baseColorTexture(),
		_normalTexture(),
		_metallicTexture(),
		_roughnessTexture(),
		_ambientOcclusionTexture(),
		_metallic(0.0f),
		_roughness(0.5f),
		_vertexShader(),
		_pixelShader(),
		_renderState()
	{

	}
}
