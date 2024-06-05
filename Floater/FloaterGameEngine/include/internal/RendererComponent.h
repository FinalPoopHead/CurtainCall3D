#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"
#include "RendererNode.h"
#include <string>


namespace flt
{
	class IRenderer;
	struct RendererObject;
	struct RawNode;

	class RendererComponent : public Component<RendererComponent>
	{
	public:
		RendererComponent();
		virtual ~RendererComponent();

		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void Update(float deltaSecond) override;
		virtual void EndDraw() override;
		//void virtual OnDisable() override;
		virtual void OnDestroy() override;

	public:
		void SetRawNode(RawNode* rawNode);
		void SetFilePath(std::wstring path);


	private:
		Resource<RendererNode> _resource;
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
		bool _isRegisted;
	};
}
