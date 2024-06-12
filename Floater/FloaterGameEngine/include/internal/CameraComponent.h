#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"


namespace flt
{
	struct RawNode;
	struct RendererObject;
	class IRenderer;

	class CameraComponent : public Component<CameraComponent>
	{
	public:
		CameraComponent();
		~CameraComponent();

		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void Update(float deltaSecond) override;
		virtual void EndDraw() override;
		virtual void OnDestroy() override;

	private:
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
	};
}
