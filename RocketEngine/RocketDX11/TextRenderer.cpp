﻿#include "TextRenderer.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

namespace Rocket::Core
{
	TextRenderer::TextRenderer()
		: _isActive(true)
		, _font(nullptr) 
		, _text("Default Text")
		, _color(DirectX::Colors::White)
		, _worldTM(Matrix::Identity)
		, _targetCameraIndex(0)
	{

	}

	TextRenderer::~TextRenderer()
	{

	}

	void TextRenderer::SetText(const std::string& str)
	{
		_text = str;
	}

	std::string& TextRenderer::GetText()
	{
		return _text;
	}

	void TextRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}

	void TextRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void TextRenderer::SetColor(Color color)
	{
		_color = color;
	}

	void TextRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		if (!_isActive)
		{
			return;
		}

		std::wstring wstr(_text.begin(), _text.end());

		Vector2 pos = Vector2(_worldTM.m[3][0], _worldTM.m[3][1]);

		// TODO : 240617 현재 임시로 텍스트의 중앙이 좌표점이 되게끔 설정해놨음.
		Vector2 origin = _font->MeasureString(wstr.c_str());
		origin /= 2.0f;
		origin = { 0.0f,0.0f };	// TODO : 240617 임시로 좌측상단으로 롤백함.

		// TODO : 240617 현재 임시로 테두리 모드를 기본으로 그리게 해놨음.
		float borderSize = 1.0f;
		_font->DrawString(spriteBatch, wstr.c_str(), pos + Vector2(borderSize, borderSize), DirectX::Colors::Black, 0.0f, origin);
		_font->DrawString(spriteBatch, wstr.c_str(), pos + Vector2(-borderSize, borderSize), DirectX::Colors::Black, 0.0f, origin);
		_font->DrawString(spriteBatch, wstr.c_str(), pos + Vector2(-borderSize, -borderSize), DirectX::Colors::Black, 0.0f, origin);
		_font->DrawString(spriteBatch, wstr.c_str(), pos + Vector2(borderSize, -borderSize), DirectX::Colors::Black, 0.0f, origin);

		_font->DrawString(
			spriteBatch
			, wstr.c_str()
			, pos
			, _color
			, 0.0f
			, origin
		);
	}

	Color TextRenderer::GetColor()
	{
		return _color;
	}

	void TextRenderer::SetFont(DirectX::SpriteFont* font)
	{
		_font = font;
	}

	void TextRenderer::SetFontwithPath(const std::string& fontPath)
	{
		_font = ResourceManager::Instance().GetFont(fontPath);
	}

	void TextRenderer::Append(const std::string& str)
	{
		_text.append(str);
	}

	void TextRenderer::Destroy()
	{
		ObjectManager::Instance().DestroyTextRenderer(this);
	}

	void TextRenderer::SetTargetCameraIndex(int cameraIndex)
	{
		_targetCameraIndex = cameraIndex;
	}

}
