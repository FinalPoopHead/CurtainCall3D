﻿#include <cassert>

#include "RocketDX11.h"
#include "Grid.h"
#include "Axis.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "GraphicsMacro.h"
#include "DeviceBuilderDX11.h"

#include "ResourceManager.h"
#include "ObjectManager.h"

#include "StaticModelRenderer.h"
#include "DynamicModelRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "GraphicsMacro.h"


namespace Rocket::Core
{
	IDX11Renderer* CreateGraphicsInstance()
	{
		return new Rocket::Core::RocketDX11();
	}

	void ReleaseGraphicsInstance(IDX11Renderer* instance)
	{
		delete instance;
	}
}

/// IRocketGraphics.h 에 있는 dllexport 함수들.
/// USE_DX11 를 전처리기에 넣어주면 DX11용 dll이 만들어진다.
/// 
/// 23.06.16 강석원 인재원.
namespace Rocket::Core
{
	RocketDX11::RocketDX11()
		: _hWnd(), _screenWidth(), _screenHeight(), _vSyncEnabled(),
		_device(), _deviceContext(),
		_featureLevel(),_m4xMsaaQuality(),
		_swapChain(), _backBuffer(),
		_renderTargetView(), _depthStencilBuffer(), _depthStencilView(),
		_viewport(),
		_resourceManager(ResourceManager::Instance()),
		_axis(), _grid(),
		_spriteBatch(), _lineBatch(), _basicEffect(),
		_lineInputLayout(),
		_deltaTime()
	{

	}

	RocketDX11::~RocketDX11()
	{

	}

	void RocketDX11::Initialize(void* hWnd, int screenWidth, int screenHeight)
	{
		// 매크로로 변경하려고 작업중
		HRESULT hr = S_OK;

		_hWnd = static_cast<HWND>(hWnd);
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		// device 빌더 클래스를 이용해 device와 deviceContext 생성.
		DeviceBuilderDX11 deviceBuilder;
		deviceBuilder.SetDevice(_device.GetAddressOf());
		deviceBuilder.SetLevelHolder(&_featureLevel);
		deviceBuilder.SetDeviceContext(_deviceContext.GetAddressOf());
		HR(deviceBuilder.Build());

		/// 아래 if문은 용책에서의 코드
		/// 버전이 11_0이 아닐때 false를 리턴한다.
		/// 내 코드에서는 11_1을 사용하는데 이래도 되는건가?!
		/// 23.04.07 강석원 인재원
	// 	if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
	// 	{
	// 		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	// 		return false;
	// 	}

		/// 멀티 샘플링 품질레벨 체크
		/// Direct11 에서는 항상 지원되므로, 반환된 품질 수준 값은 반드시 0보다 커야 한다.
		/// 23.04.07 강석원 인재원
		hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_m4xMsaaQuality);
		assert(_m4xMsaaQuality > 0);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 4;      //multisampling setting	// 나는 4x를 사용하므로 4
		swapChainDesc.SampleDesc.Quality = _m4xMsaaQuality - 1;	//vendor-specific flag	// 위에서 받아온 퀄리티 레벨을 넣어준다.	// -1 을 왜해줄까?
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;		// 이걸 2로 하게되면 렌더타겟 뷰를 각각의 버퍼에 대해 가지고 있어야하나?
		swapChainDesc.OutputWindow = _hWnd;
		swapChainDesc.Windowed = TRUE; // Sets the initial state of full-screen mode.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// 교환효과. DXGI_SWAP_EFFECT_DISCARD는 디스플레이 구동기가 가장 효율적인 제시 방법을 선택하게 함
		//desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// MSDN 그대로 따라 친 것.
		swapChainDesc.Flags = 0;

		/// DXGIDevice로 DXGIAdapter를 만들고
		/// DXGIAdapter로 DXGIFactory를 만들고
		/// DXGIFactory로 SwapChain을 만든다!
		/// 위의 swapchain 명세를 이용해 명세에 적힌대로 swapchain을 만드는 것!
		/// 
		/// 23.04.10 강석원 인재원
		// Create the DXGI device object to use in other factories, such as Direct2D.
		Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
		_device.As(&dxgiDevice);

		// Create swap chain.
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;

		hr = dxgiDevice->GetAdapter(&adapter);

		if (SUCCEEDED(hr))
		{
			adapter->GetParent(IID_PPV_ARGS(&factory));

			hr = factory->CreateSwapChain(
				_device.Get(),
				//_device.Get(),
				&swapChainDesc,
				&_swapChain
			);
		}

		hr = _swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(void**)&_backBuffer);

		hr = _device->CreateRenderTargetView(
			_backBuffer.Get(),
			nullptr,
			_renderTargetView.GetAddressOf()
		);

		D3D11_TEXTURE2D_DESC backBufferDesc;

		_backBuffer->GetDesc(&backBufferDesc);

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// 깊이 버퍼의 description을 작성합니다.
		depthBufferDesc.Width = static_cast<UINT>(backBufferDesc.Width);
		depthBufferDesc.Height = static_cast<UINT>(backBufferDesc.Height);
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = _m4xMsaaQuality - 1;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer));

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		//CD3D11_DEPTH_STENCIL_DESC depthStencilDesc();

		// 스텐실 상태의 description을 초기화합니다.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// 스텐실 상태의 description을 작성합니다.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState));

		_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 1);
		//_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 0);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// 깊이-스텐실 뷰의 description을 작성합니다.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR(_device->CreateDepthStencilView(_depthStencilBuffer.Get(), &depthStencilViewDesc, &_depthStencilView));
		//HR(_device->CreateDepthStencilView(_depthStencilBuffer.Get(), NULL, &_depthStencilView));

		//BlendState Creation
		CD3D11_BLEND_DESC tBlendDesc(D3D11_DEFAULT);
		HR(_device->CreateBlendState(&tBlendDesc, _defaultBlendState.GetAddressOf()));

		ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
		_viewport.Height = (float)backBufferDesc.Height;
		_viewport.Width = (float)backBufferDesc.Width;
		_viewport.MinDepth = 0;
		_viewport.MaxDepth = 1; 

		_deviceContext->RSSetViewports(
			1,
			&_viewport
		);

		_resourceManager.Initialize(_device.Get(), _deviceContext.Get());

		_axis = new Axis();
		_axis->Initialize(_device.Get());

		_grid = new Grid();
		_grid->Initialize(_device.Get());

		_spriteBatch = new DirectX::SpriteBatch(_deviceContext.Get());
		_lineBatch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(_deviceContext.Get());
		_basicEffect = std::make_unique<DirectX::BasicEffect>(_device.Get());
		_basicEffect->SetVertexColorEnabled(true);

		void const* shaderByteCode;
		size_t byteCodeLength;

		_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		HR(_device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
			DirectX::VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&_lineInputLayout));
	}

	void RocketDX11::BeginRender()
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = 0.1f;	// r
		color[1] = 0.1f;	// g
		color[2] = 0.1f;	// b
		color[3] = 0.1f;	// a
		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());
		
		/// RenderTargetView 와 DepthStencilBuffer를 출력 병합 단계(Output Merger Stage)에 바인딩
		_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

		_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 0);
		////Blend State Set.
		_deviceContext->OMSetBlendState(_defaultBlendState.Get(), nullptr, 0xFF);
		return;
	}

	void RocketDX11::BeginRender(float r, float g, float b, float a)
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = r;	// r
		color[1] = g;	// g
		color[2] = b;	// b
		color[3] = a;	// a
		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		/// RenderTargetView 와 DepthStencilBuffer를 출력 병합 단계(Output Merger Stage)에 바인딩
		_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

		_deviceContext->OMSetDepthStencilState(_depthStencilState.Get(), 0);
		////Blend State Set.
		//_deviceContext->OMSetBlendState(_defaultBlendState.Get(), nullptr, 0xFF);
		_deviceContext->OMSetBlendState(nullptr, nullptr, 0xFF);

		return;
	}

	void RocketDX11::RenderMesh()
	{
		Camera* mainCam = Camera::GetMainCamera();

		// 카메라 버퍼 세팅
		{
			// 버텍스 쉐이더
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(_deviceContext->Map(mainCam->GetCameraBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			CameraBufferType* cameraBufferDataPtr = (CameraBufferType*)mappedResource.pData;

			cameraBufferDataPtr->cameraPosition = mainCam->GetPosition();
			cameraBufferDataPtr->padding = 0.0f;

			_deviceContext->Unmap(mainCam->GetCameraBuffer(), 0);

			unsigned int bufferNumber = 1;

			_deviceContext->VSSetConstantBuffers(bufferNumber, 1, mainCam->GetAddressOfCameraBuffer());
		}

		for (auto meshRenderer : ObjectManager::Instance().GetStaticModelRenderers())
		{
			meshRenderer->Render(_deviceContext.Get(), mainCam->GetViewMatrix(), mainCam->GetProjectionMatrix());
		}

		for (auto skinnedMeshRenderer : ObjectManager::Instance().GetDynamicModelRenderers())
		{
			skinnedMeshRenderer->Render(_deviceContext.Get(), mainCam->GetViewMatrix(), mainCam->GetProjectionMatrix());
		}
	}

	void RocketDX11::RenderText()
	{
		_spriteBatch->Begin();
		for (auto textRenderer : ObjectManager::Instance().GetTextList())
		{
			textRenderer->Render(_spriteBatch);
		}
		_spriteBatch->End();
	}

	void RocketDX11::RenderTexture()
	{
		_spriteBatch->Begin();
		// 이미지(UI)를 그리기 위한 함수
		for (auto imageRenderer : ObjectManager::Instance().GetImageList())
		{
			imageRenderer->Render(_spriteBatch);
		}
		_spriteBatch->End();
	}

	void RocketDX11::EndRender()
	{
		// Present the back buffer to the screen since rendering is complete.
		if (_vSyncEnabled)
		{
			// Lock to screen refresh rate.
			_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			_swapChain->Present(0, 0);
		}

		return;
	}

	void Rocket::Core::RocketDX11::SetDebugMode(bool isDebug)
	{
	}

	void RocketDX11::Update(float deltaTime)
	{
		_deltaTime = deltaTime;

		Camera::GetMainCamera()->UpdateViewMatrix();
		Camera::GetMainCamera()->UpdateProjectionMatrix();
		UpdateAnimation(deltaTime);
	}

	void RocketDX11::OnResize(int _width, int _height)
	{

	}

	void RocketDX11::Render()
	{
		BeginRender(0.0f, 0.0f, 0.0f, 1.0f);
		RenderHelperObject();
		RenderMesh();

		RenderText();
		RenderTexture();
		//RenderLine();

		//_deviceContext->OMSetBlendState(nullptr, );
		//_deviceContext->OMSetDepthStencilState();

		EndRender();
	}

	void RocketDX11::Finalize()
	{
		delete _grid;
		delete _axis;
	}

	void RocketDX11::RenderHelperObject()
	{
		auto vs = _resourceManager.GetVertexShader("ColorVS");
		auto ps = _resourceManager.GetPixelShader("ColorPS");

		_grid->Update(DirectX::XMMatrixIdentity(), Camera::GetMainCamera()->GetViewMatrix(), Camera::GetMainCamera()->GetProjectionMatrix());
		_grid->Render(_deviceContext.Get(), vs->GetVertexShader(), ps->GetPixelShader(), vs->GetMatrixBuffer(), vs->GetInputLayout());
		_axis->Update(DirectX::XMMatrixIdentity(), Camera::GetMainCamera()->GetViewMatrix(), Camera::GetMainCamera()->GetProjectionMatrix());
		_axis->Render(_deviceContext.Get(), vs->GetVertexShader(), ps->GetPixelShader(), vs->GetMatrixBuffer(), vs->GetInputLayout());
	}

	void RocketDX11::RenderLine()
	{
		_basicEffect->SetWorld(DirectX::XMMatrixIdentity());
		_basicEffect->SetView(Camera::GetMainCamera()->GetViewMatrix());
		_basicEffect->SetProjection(Camera::GetMainCamera()->GetProjectionMatrix());

		_basicEffect->Apply(_deviceContext.Get());

		_deviceContext->IASetInputLayout(_lineInputLayout.Get());

		_lineBatch->Begin();

		for (const auto& line : ObjectManager::Instance().GetLineRenderer()->GetLines())
		{
			_lineBatch->DrawLine(DirectX::VertexPositionColor(line.startPos, line.color), DirectX::VertexPositionColor(line.endPos, line.color));
		}
		_lineBatch->End();
		
		ObjectManager::Instance().GetLineRenderer()->Flush();
	}

	void RocketDX11::UpdateAnimation(float deltaTime)
	{
		for (auto& dynamicModel : ObjectManager::Instance().GetDynamicModelRenderers())
		{
			dynamicModel->UpdateAnimation(deltaTime);
		}
	}

}
