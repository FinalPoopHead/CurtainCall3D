﻿#pragma once
#include <d3d11.h>
#include <memory>
#include <DirectXColors.h>
#include <string>
#include <SpriteFont.h>

#include "..\\RocketCommon\\ITextRenderer.h"

namespace Rocket::Core
{
	/// <summary>
	/// 텍스트랜더러 클래스
	/// </summary>
	class TextRenderer : public ITextRenderer
{
	public:
		TextRenderer();
		~TextRenderer();

	public:
		virtual void SetActive(bool isActive) override;
		virtual void Destroy() override;
		virtual bool IsActive() override { return _isActive; }

		virtual std::string& GetText() override;
		virtual Color GetColor() override;

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetFontwithPath(const std::string& fontPath) override;
		virtual void SetText(const std::string& str) override;
		virtual void SetColor(Color color) override;
		virtual void SetTargetCameraIndex(int cameraIndex) override;
		virtual void SetTextAlignment(eTextAlignment alignment) override;

public:
		void Render(DirectX::SpriteBatch* spriteBatch);
		void SetFont(DirectX::SpriteFont* font);
		void Append(const std::string& str);
		int GetTargetCameraIndex() { return _targetCameraIndex; }

		// 리소스매니저에서 unique_ptr로 관리.
		DirectX::SpriteFont* _font;

	private:
		// 내부 변수들
		bool _isActive;
		std::string _text;
		Color _color;
		Matrix _worldTM;
		int _targetCameraIndex;
		eTextAlignment _textAlignment;
	};
}
