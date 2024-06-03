#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"



namespace flt
{
	class IRenderer;
	struct RendererObject;
	struct RawNode;

	class RendererComponent : public Component<RendererComponent>
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
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
	};
}
