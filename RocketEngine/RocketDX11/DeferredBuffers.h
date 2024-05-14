#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

// TODO : ao, roughness, metallic을 arm 텍스쳐로 하나로 처리하는게 좋을듯?
// 1. worldPos, 2. baseColor, 3. worldSpaceNormal, 4. metallic, 5. roughness, 6. ao
const int BUFFER_COUNT = 6;		

namespace Rocket::Core
{
	class DeferredBuffers
	{
	public:
		DeferredBuffers();
		~DeferredBuffers();

	public:
		void Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear);
		void SetRenderTargets(ID3D11DeviceContext* deviceContext);
		void SetRenderTargets(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
		void SetShadowMapRenderTarget(ID3D11DeviceContext* deviceContext);
		void ReleaseRenderTargets(ID3D11DeviceContext* deviceContext);
		void ClearRenderTargets(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a);
		ID3D11ShaderResourceView* GetShaderResourceView(int index);
		ID3D11ShaderResourceView** GetAddressOfShaderResourceView(int index);
		ID3D11DepthStencilView* GetDepthStencilView() { return _depthStencilView.Get(); }
		ID3D11DepthStencilView** GetAddressOfDepthStencilView() { return _depthStencilView.GetAddressOf(); }

		ID3D11ShaderResourceView* GetShadowMapSRV() { return _shadowMapShaderResourceView.Get(); }
		ID3D11ShaderResourceView** GetAddressOfShadowMapSRV() { return _shadowMapShaderResourceView.GetAddressOf(); }

	private:
		int _textureWidth;
		int _textureHeight;
		ComPtr<ID3D11Texture2D> _renderTargetTextureArray[BUFFER_COUNT];
		ComPtr<ID3D11RenderTargetView> _renderTargetViewArray[BUFFER_COUNT];
		ComPtr<ID3D11ShaderResourceView> _shaderResourceViewArray[BUFFER_COUNT];

		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;

		ComPtr<ID3D11ShaderResourceView> _shadowMapShaderResourceView;
		ComPtr<ID3D11Texture2D> _shadowDepthBuffer;
		ComPtr<ID3D11DepthStencilView> _shadowDepthStencilView;
		D3D11_VIEWPORT _shadowMapViewport;
	};
}
