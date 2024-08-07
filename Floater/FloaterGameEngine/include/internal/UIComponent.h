﻿#pragma once
#include "Component.h"
#include "../../../FloaterUtil/include/FloaterType.h"
#include "../../../FloaterMath/include/floaterMath.h"
#include <string>
#include <memory>

namespace flt
{
	class IRenderer;
	struct RendererObject;
	class Image;
	enum class eTextAlignment;

	class UIComponent : public Component<UIComponent>
	{
	public:
		UIComponent();
		virtual ~UIComponent();

	protected:
		virtual void OnCreate() override;
		virtual void OnEnable() override;
		virtual void PreRender() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

	public:
		void SetImage(const std::wstring& filePath);
		void SetImageColor(flt::Vector4f color);
		void SetPosition(flt::Vector2f pixelPos);
		flt::Vector2f GetPosition();
		void SetOffsetPosition(flt::Vector2f offsetPos);
		flt::Vector2f GetOffsetPosition();
		bool IsOffsetMode();
		void SetZOrder(float zOrder);
		float GetZOrder();
		flt::Vector2f GetImageSize();
		void SetSize(flt::Vector2f imgSize);
		flt::Vector2f GetSize();
		void SetTextAlignment(eTextAlignment alignMode);

		void SetText(const std::wstring& text);
		void SetFont(const std::wstring& fontPath);
		void SetFontSize(int fontSize);
		void SetTextColor(flt::Vector4f color);
		void SetTextColor(float r, float g, float b);

	private:
		void UpdateGameObjectPosition();
		void SetPositionUseOffset();

	private:
		RendererObject* _rendererObject;
		IRenderer& _renderer;
		uint64 _hObject;
		bool _isDraw;
		bool _isRegisted;

		bool _isOffsetMode;

		std::unique_ptr<Image> _image;
		flt::Vector2f _size;
		flt::Vector2f _position;
		flt::Vector2f _offsetPosition;
		float _zOrder;
		eTextAlignment _textAlignment;
	};
}


