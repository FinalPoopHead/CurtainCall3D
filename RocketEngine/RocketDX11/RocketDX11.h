#pragma once
#include <windows.h>
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>

#include "../GraphicsInterface/IDX11Renderer.h"
#include "Camera.h"
#include "Light.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Grid;
	class Axis;
	class CubeMesh;
	class VertexShader;
	class PixelShader;
	class ResourceManager;
	class SpriteRenderer;
	
	class RocketDX11 final : public IDX11Renderer
	{
	public:
		RocketDX11();
		~RocketDX11();

	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) override;
		virtual void SetDebugMode(bool isDebug) override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void OnResize(int _width, int _height) override;
		virtual void Finalize() override;

	private:
		void BeginRender();
		void BeginRender(float r, float g, float b, float a);
		void RenderHelperObject();
		void RenderStaticMesh();
		void RenderText();
		void RenderLine();
		void RenderTexture();
		void EndRender();

		/// Initialize Member
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;
		bool _vSyncEnabled;
		float _deltaTime;

		/// 초기화 관련
	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;	// immediateContext
		D3D_FEATURE_LEVEL _featureLevel;

		UINT _m4xMsaaQuality;

		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11Texture2D> _backBuffer;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilState> _depthStencilState;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;
		ComPtr<ID3D11BlendState > _defaultBlendState;
		
		D3D11_VIEWPORT _viewport;

	private:
		Grid* _grid;
		Axis* _axis;
		DirectX::SpriteBatch* _spriteBatch;
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* _lineBatch;
		std::unique_ptr<DirectX::BasicEffect> _basicEffect;
		ComPtr<ID3D11InputLayout> _lineInputLayout;

	private:
		ResourceManager& _resourceManager;
	};
}
