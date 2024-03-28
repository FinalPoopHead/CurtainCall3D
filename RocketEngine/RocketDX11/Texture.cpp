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
		_texture.Reset();
		_textureView.Reset();
	}

	ID3D11ShaderResourceView* Texture::GetTextureView()
	{
		return _textureView.Get();
	}

	ID3D11ShaderResourceView** Texture::GetAddressOfTextureView()
	{
		return _textureView.GetAddressOf();
	}
}
