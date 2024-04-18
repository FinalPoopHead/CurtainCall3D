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
		Texture* GetBaseColorTexture() const { return _baseColorTexture; }
		Texture* GetNormalTexture() const { return _normalTexture; }
		Texture* GetMetallicTexture() const { return _metallicTexture; }
		Texture* GetRoughnessTexture() const { return _roughnessTexture; }
		Texture* GetAOTexture() const { return _ambientOcclusionTexture; }
		float GetMetallic() const { return _metallic; }
		float GetRoughness() const { return _roughness; }
		VertexShader* GetVertexShader() const { return _vertexShader; }
		PixelShader* GetPixelShader() const { return _pixelShader; }
		ID3D11RasterizerState* GetRenderState() const { return _renderState; }

		void SetBaseColorTexture(Texture* val) { _baseColorTexture = val; }
		void SetNormalTexture(Texture* val) { _normalTexture = val; }
		void SetMetallicTexture(Texture* val) { _metallicTexture = val; }
		void SetRoughnessTexture(Texture* val) { _roughnessTexture = val; }
		void SetAmbientOcclusionTexture(Texture* val) { _ambientOcclusionTexture = val; }
		void SetMetallic(float val) { _metallic = val; }
		void SetRoughness(float val) { _roughness = val; }
		void SetVertexShader(VertexShader* val) { _vertexShader = val; }
		void SetPixelShader(PixelShader* val) { _pixelShader = val; }
		void SetRenderState(ID3D11RasterizerState* val) { _renderState = val; }
	
	private:
		// TODO : AO, Roughness, Metallic을 ARM으로 합쳐야되려나?
		Texture* _baseColorTexture;
		Texture* _normalTexture;
		Texture* _metallicTexture;
		Texture* _roughnessTexture;
		Texture* _ambientOcclusionTexture;

		float _metallic;
		float _roughness;

		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		ID3D11RasterizerState* _renderState;
	};
}
