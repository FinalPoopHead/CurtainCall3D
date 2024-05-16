#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <string>

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
		void LoadFromFile(ID3D11Device* device, std::string fileName);
		void SetResource(ID3D11Resource* resource);
		void SetSRV(ID3D11ShaderResourceView* srv);
		ID3D11Resource* GetResource();
		ID3D11Resource** GetAddressOfResource();
		ID3D11ShaderResourceView* GetSRV();
		ID3D11ShaderResourceView** GetAddressOfSRV();

	private:
		ComPtr<ID3D11Resource> _texture;
		ComPtr<ID3D11ShaderResourceView> _textureSRV;
	};
}
