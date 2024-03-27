#pragma once
#include <string>
#include "DLLExporter.h"
#include "ModelRendererBase.h"
#include "../RocketCommon/GraphicsEnum.h"

namespace Rocket::Core
{
	class IMeshRenderer;
}

namespace Rocket
{
	class GameObject;

	/// <summary>
	/// Static한 Mesh만을 출력해주는 Component.
	/// Dynamic한 Model을 출력하고 싶다면, DynamicModelRenderer를 사용하자.
	/// </summary>
	class ROCKET_API MeshRenderer final : public Core::ModelRendererBase
	{
	public:
		MeshRenderer();
		//void SetMesh(eMeshType meshType);
		void LoadMesh(std::string fileName);
		void SetTexture(std::string fileName);

	protected:
		virtual void UpdateRenderData() override;

	private:
		Core::IMeshRenderer* _graphicsRenderer;
	};
}

