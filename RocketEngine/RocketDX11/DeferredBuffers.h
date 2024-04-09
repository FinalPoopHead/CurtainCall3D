#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

const int BUFFER_COUNT = 4;

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
		void ReleaseRenderTargets(ID3D11DeviceContext* deviceContext);
		void ClearRenderTargets(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a);
		ID3D11ShaderResourceView* GetShaderResourceView(int index);
		ID3D11ShaderResourceView** GetAddressOfShaderResourceView(int index);
		ID3D11DepthStencilView* GetDepthStencilView() { return _depthStencilView.Get(); }
		ID3D11DepthStencilView** GetAddressOfDepthStencilView() { return _depthStencilView.GetAddressOf(); }

	private:
		int _textureWidth;
		int _textureHeight;
		ComPtr<ID3D11Texture2D> _renderTargetTextureArray[BUFFER_COUNT];
		ComPtr<ID3D11RenderTargetView> _renderTargetViewArray[BUFFER_COUNT];
		ComPtr<ID3D11ShaderResourceView> _shaderResourceViewArray[BUFFER_COUNT];

		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;
	};
}
