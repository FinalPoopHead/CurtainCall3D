#pragma once
#include <string>
#include "IRenderable.h"
#include "GraphicsEnum.h"

namespace Rocket::Core
{
	class IStaticModelRenderer : public IRenderable
	{
	public:
		virtual ~IStaticModelRenderer() {}
		// TODO : 외부에서 기본 도형들은 편하게 Set 할 수 있게 하면 좋을듯.
		// virtual void LoadModel(eMeshType meshType) = 0;
		virtual void LoadModel(std::string fileName) = 0;
		// TODO : 나중에는 머터리얼단위로 Set 할 예정.
		// virtual void SetMaterial(std::string fileName) = 0;
		virtual void LoadTexture(std::string fileName) = 0;		// 현재 머터리얼의 텍스쳐를 Set하는 함수.

	};
}
