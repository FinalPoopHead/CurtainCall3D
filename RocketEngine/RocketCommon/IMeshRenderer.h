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
	/// Static Mesh만을 그리는 컴포넌트의 인터페이스.
	/// 프레임워크에서는 이 인터페이스를 이용해서 그래픽스의 MeshRenderer를 다룬다.
	class IMeshRenderer : public IGraphicsObject
	{
	public:
		virtual ~IMeshRenderer() {}
		// TODO : 외부에서 기본 도형들은 편하게 Set 할 수 있게 하면 좋을듯.
		// virtual void LoadModel(eMeshType meshType) = 0;
		virtual void LoadMesh(std::string fileName) = 0;
		// TODO : 나중에는 머터리얼단위로 Set 할 예정.
		// virtual void SetMaterial(std::string fileName) = 0;
		virtual void LoadTexture(std::string fileName) = 0;		// 현재 머터리얼의 텍스쳐를 Set하는 함수.
		virtual void BindTransform(RocketTransform* transform) = 0;

	};
}
