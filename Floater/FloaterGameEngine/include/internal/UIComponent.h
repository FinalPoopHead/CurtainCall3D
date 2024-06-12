#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"
#include "../../../FloaterMath/include/Vector2f.h"
#include <string>
#include <memory>

namespace flt
{
	class IRenderer;
	struct RendererObject;
	class Image;

	class UIComponent : public Component<UIComponent>
	{
	public:
		UIComponent();
		virtual ~UIComponent();

	protected:
		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

	public:
		void SetImage(const std::wstring& filePath);
		void SetPosition(float pixelX, float pixelY);
		void GetImageSize(float& width, float& height);
		void SetSize(float width, float height);
		void GetSize(float& width, float& height);

	private:
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
		bool _isRegisted;

		std::unique_ptr<Image> _image;
		flt::Vector2f _size;
		flt::Vector2f _position;
	};
}


