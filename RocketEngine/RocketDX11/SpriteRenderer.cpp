#include <wrl.h>
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	SpriteRenderer::SpriteRenderer()
		: _imageWidth(),
		_imageHeight()
	{
		_color = DirectX::Colors::White;
	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::SetImage(const std::string& fileName)
	{
		_texture = ResourceManager::Instance().GetTexture(fileName);

		D3D11_TEXTURE2D_DESC textureDesc;
		ComPtr<ID3D11Resource> resource;
		_texture->GetTextureView()->GetResource(resource.GetAddressOf());
		ComPtr<ID3D11Texture2D> texture2D = static_cast<ID3D11Texture2D*>(resource.Get());
		texture2D->GetDesc(&textureDesc);

		_imageWidth = static_cast<float>(textureDesc.Width);
		_imageHeight = static_cast<float>(textureDesc.Height);
	}

	void SpriteRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		Vector3 scale;
		Quaternion quat;
		Vector3 pos;

		_worldTM.Decompose(scale, quat, pos);

		spriteBatch->Draw(
			_texture->GetTextureView(),
			DirectX::XMFLOAT2(pos.x, pos.y),
			nullptr,
			_color,
			0.0f,										//회전 각도
			DirectX::XMFLOAT2(0.5f, 0.5f),				//  이미지의 원점->0.0f,0.0f이면 좌측상단
			DirectX::XMFLOAT2(scale.x, scale.y));		// 이미지 스케일
	}

	void SpriteRenderer::SetColor(Color color)
	{
		_color = color;
	}

	void SpriteRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}


	void SpriteRenderer::SetActive(bool isActive)
	{
		isActive = 1;
	}
}
