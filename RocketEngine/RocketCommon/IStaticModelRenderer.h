#pragma once
#include <string>
#include "IGraphicsObject.h"
#include "GraphicsEnum.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	/// Model 단위로 그리는 Static Mesh 컴포넌트
	/// MeshRenderer와의 차이점은, MeshRenderer는 게임오브젝트 별로 붙는거에 반해
	/// 얘는 최상위 게임오브젝트에 붙어서 하위 Mesh들을 전부 그려준다.
	class IStaticModelRenderer : public IGraphicsObject
	{
	public:
		virtual ~IStaticModelRenderer() {}
		// TODO : 외부에서 기본 도형들은 편하게 Set 할 수 있게 하면 좋을듯.
		virtual void LoadModel(std::string fileName) = 0;
		// TODO : 나중에는 머터리얼단위로 Set 할 예정.
		// virtual void SetMaterial(std::string fileName) = 0;
		virtual void LoadBaseColorTexture(std::string fileName) = 0;		// 현재 머터리얼의 텍스쳐를 Set하는 함수.
		virtual void LoadNormalTexture(std::string fileName) = 0;
		virtual void LoadMetallicTexture(std::string fileName) = 0;
		virtual void LoadRoughnessTexture(std::string fileName) = 0;
		virtual void LoadAOTexture(std::string fileName) = 0;
		virtual void BindTransform(RocketTransform* transform) = 0;

		// 임시
		virtual void SetMetallic(float metallic) = 0;
		virtual void SetRoughness(float roughness) = 0;
	};
}
