#pragma once
#include <string>
#include "DLLExporter.h"
#include "ModelRendererBase.h"
#include "../RocketCommon/GraphicsEnum.h"

namespace Rocket::Core
{
	class IStaticModelRenderer;
}

namespace Rocket
{
	class GameObject;

	/// <summary>
	/// 테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용테스트용
	/// </summary>
	class ROCKET_API StaticModelRenderer final : public Core::ModelRendererBase
	{
	public:
		StaticModelRenderer();
		void LoadModel(std::string fileName);
		void SetBaseColorTexture(std::string fileName);
		void SetNormalTexture(std::string fileName);
		void SetMetallicTexture(std::string fileName);
		void SetRoughnessTexture(std::string fileName);
		void SetAOTexture(std::string fileName);
		void SetMetallic(float value);
		void SetRoughness(float value);

		virtual void BindTransform() override;

	protected:
		virtual void UpdateRenderData() override;

	private:
		Core::IStaticModelRenderer* _graphicsRenderer;
	};
}

