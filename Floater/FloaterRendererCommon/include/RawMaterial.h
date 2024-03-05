#pragma once
#include "RawTexture.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include <string>

namespace flt
{
	struct RawMaterial
	{
		static constexpr int MAX_TEXTURES = 10;

		enum TextureType
		{
			ALBEDO_OPACITY = 0,
			NORMAL,
			METALLIC,
			ROUGHNESS,
			AO,
			EMISSIVE,
			HEIGHT,
			OPACITY,
			UNKNOWN
		};

		RawMaterial() :
			name(),
			textures{},
			baseColor{ 0.0f, 0.0f, 0.0f }, specular{ 0.0f, 0.0f, 0.0f }, emissiveColor{ 0.0f, 0.0f, 0.0f }, ambient{ 0.0f, 0.0f, 0.0f }, metallic(0.0f), roughness(0.0f), opacity(0.0f)
		{
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				textures[i] = new(std::nothrow) RawTexture();
			}
		}
		RawMaterial(const RawMaterial& other)
		{
			name = other.name;
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				textures[i] = new(std::nothrow) RawTexture();

				if (!textures[i])
				{
					ASSERT(false, "Failed to allocate memory for RawTexture");
					continue;
				}
				textures[i]->name = other.textures[i]->name;
				textures[i]->path = other.textures[i]->path;
			}
			for (int i = 0; i < 3; ++i)
			{
				baseColor[i] = other.baseColor[i];
				specular[i] = other.specular[i];
				emissiveColor[i] = other.emissiveColor[i];
				ambient[i] = other.ambient[i];
			}
			metallic = other.metallic;
			roughness = other.roughness;
			opacity = other.opacity;
		}

		~RawMaterial()
		{
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				delete textures[i];
			}
		}

		RawMaterial& operator=(const RawMaterial& other)
		{
			name = other.name;
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				textures[i] = new(std::nothrow) RawTexture();

				if (!textures[i])
				{
					ASSERT(false, "Failed to allocate memory for RawTexture");
					continue;
				}
				textures[i]->name = other.textures[i]->name;
				textures[i]->path = other.textures[i]->path;
			}
			for (int i = 0; i < 3; ++i)
			{
				baseColor[i] = other.baseColor[i];
				specular[i] = other.specular[i];
				emissiveColor[i] = other.emissiveColor[i];
				ambient[i] = other.ambient[i];
			}
			metallic = other.metallic;
			roughness = other.roughness;
			opacity = other.opacity;

			return *this;
		}

		std::wstring name;

		RawTexture* textures[MAX_TEXTURES];

		float baseColor[3];
		float specular[3];
		float emissiveColor[3];
		float ambient[3];
		float metallic;
		float roughness;
		float opacity;
	};
}
