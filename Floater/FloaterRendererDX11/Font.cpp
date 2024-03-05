#include "Font.h"

// 웹에서 폰트 다운받고, spriteFont Github에서 exe파일 다운로드
// 만드는 명령어가 있는데 git에 있겠지 뭐.
// 이후 생성할 때 경로를 지정해주면 됨.
//

flt::Font::Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::wstring& filePath) :
	_pDevice(pDevice),
	_pContext(pContext),
	_pFont(new DirectX::SpriteFont(_pDevice, filePath.c_str())),
	_pSpriteBatch(new DirectX::SpriteBatch(_pContext))
{
}

flt::Font::~Font()
{

}

void flt::Font::RenderText(const std::wstring& text, float screenX, float screenY)
{
	_pSpriteBatch->Begin();
	_pFont->DrawString(_pSpriteBatch.get(), text.c_str(), DirectX::XMFLOAT2(screenX, screenY));
	_pSpriteBatch->End();
}
