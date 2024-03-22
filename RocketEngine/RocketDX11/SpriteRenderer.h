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

		virtual void SetImage(const std::string& fileName) override;
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetColor(Color color) override;

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

	private:
		Color _color;
		Matrix _worldTM;

		// 이미지 정보
		float _imageWidth;
		float _imageHeight;

		Texture* _texture;
	};
}
