#pragma once
#include <string>
#include "IRenderable.h"
#include "GraphicsEnum.h"

namespace Rocket::Core
{
	class IMeshRenderer : public IRenderable
	{
	public:
		virtual ~IMeshRenderer() {}
		virtual void LoadMesh(eMeshType meshType) = 0;
		virtual void LoadMesh(std::string fileName) = 0;
		virtual void LoadTexture(std::string fileName) = 0;

	};
}
