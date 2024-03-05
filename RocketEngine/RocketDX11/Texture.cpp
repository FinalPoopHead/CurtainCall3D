#include "Texture.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	Texture::Texture()
		: _texture(nullptr),
		_textureView(nullptr)
	{

	}

	Texture::Texture(const Texture& other)
		: _texture(other._texture),
		_textureView(other._textureView)
	{

	}

	Texture::Texture(ID3D11Resource* texture, ID3D11ShaderResourceView* textureView)
		: _texture(texture),
		_textureView(textureView)
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::Shutdown()
	{
		// 텍스쳐 뷰 리소스 해제
		if (_textureView)
		{
			_textureView->Release();
			_textureView = nullptr;
		}

		if (_texture)
		{
			_texture->Release();
			_texture = nullptr;
		}
	}

	ID3D11ShaderResourceView* Texture::GetTexture()
	{
		return _textureView.Get();
	}

	ID3D11ShaderResourceView** Texture::GetAddressOfTextureView()
	{
		return _textureView.GetAddressOf();
	}
}
