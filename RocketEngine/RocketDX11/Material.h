#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

namespace Rocket::Core
{
	class Material
	{
	public:
		Material();

	public:
		Texture* GetTexture() const { return _texture; }
		VertexShader* GetVertexShader() const { return _vertexShader; }
		PixelShader* GetPixelShader() const { return _pixelShader; }
		ID3D11RasterizerState* GetRenderState() const { return _renderState; }

		void SetTexture(Texture* val) { _texture = val; }
		void SetVertexShader(VertexShader* val) { _vertexShader = val; }
		void SetPixelShader(PixelShader* val) { _pixelShader = val; }
		void SetRenderState(ID3D11RasterizerState* val) { _renderState = val; }
	
	private:
		Texture* _texture;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		ID3D11RasterizerState* _renderState;
	};
}
