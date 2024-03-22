#pragma once
#include <string>
#include "MathHeader.h"

namespace Rocket::Core
{
	class IMaterial
	{
	public:
		virtual void SetMaterialName(const std::string& materialName) = 0;
		virtual void SetVertexShader(const std::string& vertexShader) = 0;
		virtual void SetPixelShader(const std::string& pixelShader) = 0;
		virtual void SetTexture(const unsigned int index, const std::string& texture) = 0;
		virtual void SetColor(const Color color) = 0;

		virtual const std::string& GetMaterialName() const = 0;
	};

	struct MaterialDesc
	{
		std::string materialName;
		std::string vertexShader;
		std::string pixelShader;
		std::string texture;
		unsigned int textureIndex;
		Color color;
	};
}