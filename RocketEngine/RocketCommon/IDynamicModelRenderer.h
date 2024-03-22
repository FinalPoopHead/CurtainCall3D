#pragma once
#include <string>
#include "IRenderable.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	class IDynamicModelRenderer : public IRenderable
	{
	public:
		virtual ~IDynamicModelRenderer() {}
		virtual void LoadModel(const std::string& fileName) = 0;
		virtual void LoadTexture(std::string fileName) = 0;
		virtual void BindTransform(RocketTransform* rootTransform) = 0;
// 		virtual void LoadNormalMap(const std::string& fileName) = 0;
// 		virtual void LoadDiffuseMap(const std::string& fileName) = 0;
// 		virtual void LoadVertexShader(const std::string& fileName) = 0;
// 		virtual void LoadPixelShader(const std::string& fileName) = 0;
// 		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) = 0;
// 		virtual void PlayAnimation(UINT index, bool isLoop = true) = 0;
	};
}
