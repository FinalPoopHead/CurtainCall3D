#pragma once
#include <string>
#include "DLLExporter.h"
#include "ModelRendererBase.h"
#include "../GraphicsInterface/GraphicsEnum.h"

namespace Rocket::Core
{
	class IStaticModelRenderer;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API StaticModelRenderer final : public Core::ModelRendererBase
	{
	public:
		StaticModelRenderer();
		//void SetMesh(eMeshType meshType);
		void SetMesh(std::string fileName);
		void SetTexture(std::string fileName);

	protected:
		virtual void UpdateRenderData() override;

	private:
		Core::IStaticModelRenderer* _meshRenderer;
	};
}

