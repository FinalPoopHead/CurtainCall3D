﻿#include <wrl.h>
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "ObjectManager.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	SpriteRenderer::SpriteRenderer() : 
		_isActive(true)
		, _color()
		, _worldTM()
		, _imageWidth()
		, _imageHeight()
		, _texture()
		, _targetCameraIndex(0)
	{
		_color = DirectX::Colors::White;
	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::SetImage(const std::string& filePath)
	{
		_texture = ResourceManager::Instance().GetTexture(filePath);

		D3D11_TEXTURE2D_DESC textureDesc;
		ComPtr<ID3D11Resource> resource;
		_texture->GetSRV()->GetResource(resource.GetAddressOf());
		ComPtr<ID3D11Texture2D> texture2D = static_cast<ID3D11Texture2D*>(resource.Get());
		texture2D->GetDesc(&textureDesc);

		_imageWidth = static_cast<float>(textureDesc.Width);
		_imageHeight = static_cast<float>(textureDesc.Height);
	}

	void SpriteRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		if (!_isActive)
		{
			return;
		}

		Vector3 scale;
		Quaternion quat;
		Vector3 pos;

		_worldTM.Decompose(scale, quat, pos);
		
		spriteBatch->Draw(
			_texture->GetSRV()
			, DirectX::XMFLOAT2(pos.x, pos.y)
			, nullptr
			, _color
			, 0.0f										// 회전 각도
			, DirectX::XMFLOAT2(0.0f, 0.0f)				// 이미지의 기준점 0,0 이면 좌측상단 (픽셀값인듯)
			, DirectX::XMFLOAT2(scale.x, scale.y)		// 이미지 스케일
			, DirectX::SpriteEffects::SpriteEffects_None
			, pos.z										// 이미지 Layer depth
		);		
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
		_isActive = isActive;
	}

	void SpriteRenderer::Destroy()
	{
		ObjectManager::Instance().DestroySpriteRenderer(this);
	}

	void SpriteRenderer::SetTargetCameraIndex(int cameraIndex)
	{
		_targetCameraIndex = cameraIndex;
	}

}
