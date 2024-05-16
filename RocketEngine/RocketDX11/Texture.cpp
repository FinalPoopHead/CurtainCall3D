#include <cassert>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <DirectXTex.h>
#include <io.h>

#include "Texture.h"
#include "ResourcePath.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	template <typename T>
	ULONG GetRefCount(const ComPtr<T>& p)
	{
		T* temp = p.Get();

		ULONG ret = 0;
		if (temp != nullptr)
		{
			ret = temp->AddRef();
			ret = temp->Release();
		}

		return ret;
	}
}

namespace Rocket::Core
{
	Texture::Texture()
		: _texture(nullptr),
		_textureSRV(nullptr)
	{

	}

	Texture::Texture(ID3D11Resource* texture, ID3D11ShaderResourceView* textureView)
		: _texture(texture), _textureSRV(textureView)
	{

	}

	Texture::Texture(const Texture& other)
		: _texture(other._texture), _textureSRV(other._textureSRV)
	{

	}

	Texture::~Texture()
	{
		_texture.Reset();
		_textureSRV.Reset();
	}

	void Texture::LoadFromFile(ID3D11Device* device, std::string fileName)
	{
		std::string fullPath = TEXTURE_PATH + fileName;
		std::wstring wFileName(fullPath.begin(), fullPath.end());

		std::string extension = fileName.substr(fileName.find_last_of(".") + 1);

		if (_access(fullPath.c_str(), 0) == -1)
		{
			return;
		}
		
		if (extension == "dds")
		{
			HR(DirectX::CreateDDSTextureFromFile(device, wFileName.c_str(), _texture.GetAddressOf(), _textureSRV.GetAddressOf()));
		}
		else if (extension == "jpg" || extension == "png")
		{
			HR(DirectX::CreateWICTextureFromFile(device, wFileName.c_str(), _texture.GetAddressOf(), _textureSRV.GetAddressOf()));
		}
		else if (extension == "tga")
		{
			DirectX::ScratchImage image;
			HR(DirectX::LoadFromTGAFile(wFileName.c_str(), nullptr, image));
			HR(CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), _textureSRV.GetAddressOf()));
		}
		else
		{
			assert(false);
		}

		if(_texture)
		{
			_texture->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"TEXTUREtexture") - 1, L"TEXTUREtexture");
		}

		if (_textureSRV)
		{
			_textureSRV->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"TEXTUREtextureView") - 1, L"TEXTUREtextureView");
		}
	}

	ID3D11ShaderResourceView* Texture::GetSRV()
	{
		return _textureSRV.Get();
	}

	ID3D11ShaderResourceView** Texture::GetAddressOfSRV()
	{
		return _textureSRV.GetAddressOf();
	}

	ID3D11Resource* Texture::GetResource()
	{
		return _texture.Get();
	}

	ID3D11Resource** Texture::GetAddressOfResource()
	{
		return _texture.GetAddressOf();
	}

	void Texture::SetResource(ID3D11Resource* resource)
	{
		_texture = resource;
	}

	void Texture::SetSRV(ID3D11ShaderResourceView* srv)
	{
		_textureSRV = srv;
	}

}
