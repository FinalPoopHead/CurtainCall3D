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

	class ROCKET_API DynamicModelRenderer final : public Core::ModelRendererBase
	{
	public:
		DynamicModelRenderer();
		void SetMesh(std::string fileName);
		void SetTexture(std::string fileName);

	protected:
		virtual void UpdateRenderData() override;

	private:
		Core::IDynamicModelRenderer* _skinnedMeshRenderer;
	};
}
