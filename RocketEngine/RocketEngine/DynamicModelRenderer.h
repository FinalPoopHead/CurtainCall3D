#pragma once
#include <string>

#include "DLLExporter.h"
#include "ModelRendererBase.h"

namespace Rocket::Core
{
	class IDynamicModelRenderer;
}

namespace Rocket
{
	class GameObject;
	class Transform;
}

namespace Rocket
{
	/// <summary>
	/// Skinning이 적용된 애니메이션이 존재하는 Model을 출력하는 컴포넌트.
	/// 단순 Mesh만 출력하고 싶다면 MeshRenderer를 사용하자.
	/// </summary>
	class ROCKET_API DynamicModelRenderer final : public Core::ModelRendererBase
	{
	public:
		DynamicModelRenderer();
		void LoadModel(std::string fileName);
		void SetTexture(std::string fileName);

	protected:
		virtual void UpdateRenderData() override;

	private:
		Core::IDynamicModelRenderer* _graphicsComponent;
	};
}
