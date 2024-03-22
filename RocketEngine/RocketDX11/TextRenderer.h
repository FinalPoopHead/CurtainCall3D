#pragma once
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

		virtual std::string& GetText() override;
		virtual Color GetColor() override;

		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetText(const std::string& str) override;
		virtual void SetColor(Color color) override;

public:
		void Render(DirectX::SpriteBatch* spriteBatch);

		DirectX::SpriteFont* _font;

	private:
		// 내부 변수들
		bool _isActive;
		std::string _text;
		Color _color;
		Matrix _worldTM;
	};
}
