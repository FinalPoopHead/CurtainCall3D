#pragma once
#include "Component.h"
#include "../../FloaterRendererCommon/include/IRenderer.h"

namespace flt
{
	struct RawNode;

	class CameraComponent : public Component<CameraComponent>
	{
	public:
		CameraComponent(GameObject* gameObject);

		void virtual OnEnable() override;
		void virtual Update(float deltaSecond) override;
		void virtual EndDraw() override;
		void virtual OnDestroy() override;

	private:
		RendererObject _rendererObject;
		IRenderer& _renderer;
		HOBJECT _hObject;
		bool _isDraw;
	};
}
