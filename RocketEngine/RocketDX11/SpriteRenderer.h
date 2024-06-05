#pragma once
#include <SpriteBatch.h>

#include "..\\RocketCommon\\ISpriteRenderer.h"
#include "MathHeader.h"

namespace Rocket::Core
{
	class Texture;
}

namespace Rocket::Core
{
	class SpriteRenderer : public Rocket::Core::ISpriteRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void Destroy() override;
		virtual void SetActive(bool isActive) override;
		virtual bool IsActive() override { return _isActive; }

		virtual void SetImage(const std::string& fileName) override;
		virtual void SetColor(Color color) override;

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

	private:
		bool _isActive;

		Color _color;
		Matrix _worldTM;

		// 이미지 정보
		float _imageWidth;
		float _imageHeight;

		Texture* _texture;		// Resource Manager에서 unique_ptr로 관리 중.
	};
}
