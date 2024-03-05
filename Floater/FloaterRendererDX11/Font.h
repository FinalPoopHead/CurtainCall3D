#pragma once

#include "../External/include/directxtk/SpriteBatch.h"
#include "../External/include/directxtk/SpriteFont.h"

#include <string>
#include <d3d11_4.h>


namespace flt
{
	class Font
	{
	public:
		Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::wstring& filePath);
		~Font();

		//void LoadFont(std::string fontName, std::string fontPath);
		void RenderText(const std::wstring& text, float screenX, float screenY);
	private:
		ID3D11Device* _pDevice;
		ID3D11DeviceContext* _pContext;
		std::unique_ptr<DirectX::SpriteFont> _pFont;
		std::unique_ptr<DirectX::SpriteBatch> _pSpriteBatch;
	};
}
