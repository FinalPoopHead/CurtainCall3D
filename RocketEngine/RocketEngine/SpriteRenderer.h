#pragma once
#include <string>
#include "DLLExporter.h"
#include "UIRenderer.h"

namespace Rocket::Core
{
	class ISpriteRenderer;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API SpriteRenderer : public Rocket::Core::UIRenderer
	{
	public:
		SpriteRenderer();

	protected:
		virtual void UpdateRenderData() override;

	public:
		void SetPath(std::string path);

	private:
		Core::ISpriteRenderer* _spriteRenderer;
	};
}
