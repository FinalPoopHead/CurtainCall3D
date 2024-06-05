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

	public:
		virtual void LoadModel(const std::string& fileName) = 0;
		virtual void LoadBaseColorTexture(std::string fileName) = 0;
		virtual void LoadNormalTexture(std::string fileName) = 0;
		virtual void LoadMetallicTexture(std::string fileName) = 0;
		virtual void LoadRoughnessTexture(std::string fileName) = 0;
		virtual void LoadAOTexture(std::string fileName) = 0;
		virtual void BindTransform(RocketTransform* rootTransform) = 0;

		// 애니메이션
		virtual void StopAnimation() = 0;
		virtual void ForceStopAnimation() = 0;
		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) = 0;
		virtual void PlayAnimation(UINT index, bool isLoop = true) = 0;
		virtual std::string GetAnimName(UINT index) = 0;				// 없으면 return ""; 있으면 해당 애니메이션 이름(문자열)
		virtual int GetAnimIndex(const std::string& animName) = 0;		// 없으면 return -1; 있으면 해당 인덱스
		virtual std::string GetCurrentAnimName() = 0;
		virtual UINT GetAnimCount() = 0;

		// 임시
		virtual void SetMetallic(float metallic) = 0;
		virtual void SetRoughness(float roughness) = 0;

// 		virtual void LoadVertexShader(const std::string& fileName) = 0;
// 		virtual void LoadPixelShader(const std::string& fileName) = 0;
// 		virtual void PlayAnimation(const std::string& animName, bool isLoop = true) = 0;
// 		virtual void PlayAnimation(UINT index, bool isLoop = true) = 0;
	};
}
