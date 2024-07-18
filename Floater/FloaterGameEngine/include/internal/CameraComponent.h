#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"
#include "../../../FloaterMath/include/Matrix4f.h"
#include <functional>


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

		uint32 SetIndex(uint32 priority);
		Matrix4f GetViewMatrix() const;
		Matrix4f GetProjectionMatrix() const;

	protected:
		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void Update(float deltaSecond) override;
		virtual void PostRender() override;
		virtual void OnDestroy() override;

	private:
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
	};
}
