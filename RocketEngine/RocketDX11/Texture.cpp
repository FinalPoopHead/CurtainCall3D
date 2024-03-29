#include <cassert>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

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
		_textureView(nullptr)
	{

	}

	Texture::~Texture()
	{
		_texture.Reset();
		_textureView.Reset();
	}

	void Texture::LoadFromFile(ID3D11Device* device, std::string fileName)
	{
		std::string fullPath = TEXTURE_PATH + fileName;
		std::wstring wFileName(fullPath.begin(), fullPath.end());

		std::string extension = fileName.substr(fileName.find_last_of(".") + 1);

		if (extension == "dds")
		{
			HR(DirectX::CreateDDSTextureFromFile(device, wFileName.c_str(), _texture.GetAddressOf(), _textureView.GetAddressOf()));
		}
		else if (extension == "jpg" || extension == "png")
		{
			HR(DirectX::CreateWICTextureFromFile(device, wFileName.c_str(), _texture.GetAddressOf(), _textureView.GetAddressOf()));
		}
		else
		{
			assert(false);
		}

		if(_texture)
		{
			_texture->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"TEXTUREtexture") - 1, L"TEXTUREtexture");
		}

		if (_textureView)
		{
			_textureView->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"TEXTUREtextureView") - 1, L"TEXTUREtextureView");
		}
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
