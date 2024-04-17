#pragma once
#include "Component.h"
#include "../../FloaterRendererCommon/include/IRenderer.h"


namespace flt
{
	class IRenderer;
	struct RawNode;

	class RendererComponent : public ComponentBase<RendererComponent>
	{
	public:
		RendererComponent(GameObject* gameObject);
		virtual ~RendererComponent();

		void virtual OnEnable() override;
		void virtual Update(float deltaSecond) override;
		void virtual EndDraw() override;
		//void virtual OnDisable() override;
		void virtual OnDestroy() override;

	public:
		void SetRawNode(RawNode* rawNode);


	private:
		RendererObject _rendererObject;
		IRenderer& _renderer;
		HOBJECT _hObject;
		bool _isDraw;
	};
}
