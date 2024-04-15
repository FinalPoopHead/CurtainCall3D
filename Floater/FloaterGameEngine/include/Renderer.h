#pragma once
#include "Component.h"
#include "../FloaterRendererCommon/include/IRenderer.h"


namespace flt
{
	class IRenderer;

	class Renderer : public Component
	{
	public:
		Renderer(GameObject& gameObject);
		virtual ~Renderer();

		void virtual OnEnable() override;
		void virtual Update(float deltaSecond) override;
		void virtual EndDraw() override;
		void virtual OnDisable() override;
		void virtual OnDestroy() override;

	private:
		GameObject& _gameObject;
		IRenderer& _renderer;
		HOBJECT _hObject;
		bool _isDraw;
	};
}
