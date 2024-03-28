#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

#include "IResource.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Texture : public IResource
	{
	public:
		Texture();
		Texture(ID3D11Resource* texture, ID3D11ShaderResourceView* textureView);
		Texture(const Texture& other);
		~Texture();

	public:
		ID3D11ShaderResourceView* GetTextureView();
		ID3D11ShaderResourceView** GetAddressOfTextureView();

	private:
		ComPtr<ID3D11Resource> _texture;
		ComPtr<ID3D11ShaderResourceView> _textureView;
	};
}
