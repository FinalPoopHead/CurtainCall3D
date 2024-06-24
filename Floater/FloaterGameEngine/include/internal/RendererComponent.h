#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"
#include "RendererNode.h"
#include "../../../FloaterRendererCommon/include/RawNode.h"
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
		virtual void OnDestroy() override;

	public:
		void SetText(const std::wstring& text);
		void SetFont(const std::wstring& path);
		void SetTextSize(uint32 size);
		void SetImage(const std::wstring& path);
		void SetFilePath(const std::wstring& path);
		void SetMaterial(uint32 meshIndex, const std::wstring& path, RawMaterial::TextureType type);
		std::vector<std::wstring> GetAnimaionList() const;

		//void PlayAnimation(const std::wstring& name);
		void PlayAnimation(uint32 index, bool isLoop);
		void StopAnimation();

	private:
		void SetRawNode(RawNode* rawNode);

	private:
		Resource<RendererNode> _resource;
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
		bool _isRegisted;
	};
}
