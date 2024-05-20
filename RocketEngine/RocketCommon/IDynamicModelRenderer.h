#pragma once
#include <string>
#include "IGraphicsObject.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	class IDynamicModelRenderer : public IGraphicsObject
	{
	public:
		virtual ~IDynamicModelRenderer() {}
		virtual void LoadModel(const std::string& fileName) = 0;
		virtual void LoadBaseColorTexture(std::string fileName) = 0;
		virtual void LoadNormalTexture(std::string fileName) = 0;
		virtual void LoadMetallicTexture(std::string fileName) = 0;
		virtual void LoadRoughnessTexture(std::string fileName) = 0;
		virtual void LoadAOTexture(std::string fileName) = 0;
		virtual void BindTransform(RocketTransform* rootTransform) = 0;

		// 임시
		virtual void SetMetallic(float metallic) = 0;
		virtual void SetRoughness(float roughness) = 0;

// 		virtual void LoadVertexShader(const std::string& fileName) = 0;
// 		virtual void LoadPixelShader(const std::string& fileName) = 0;
// 		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) = 0;
// 		virtual void PlayAnimation(UINT index, bool isLoop = true) = 0;
	};
}
