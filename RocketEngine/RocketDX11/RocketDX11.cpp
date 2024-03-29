#include <cassert>

#include "RocketDX11.h"
#include "Camera.h"
#include "Grid.h"
#include "Axis.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "CubeMap.h"

#include "GraphicsMacro.h"

#include "ObjectManager.h"
#include "ResourceManager.h"

#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	template <typename T>
	ULONG GetRefCount(const ComPtr<T>& p)
	{
		T* temp = p.Get();

		ULONG ret = 0;
		if (temp != nullptr)
		{
			ret = temp->AddRef();
			ret = temp->Release();
		}

		return ret;
	}
}

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
		_objectManager(ObjectManager::Instance()),
		_resourceManager(ResourceManager::Instance()),
		_spriteBatch(), _lineBatch(), _basicEffect(),
		_lineInputLayout(),
		_deltaTime(),
		_isDebugMode(false)
	{

	}

	RocketDX11::~RocketDX11()
	{

	}

	void RocketDX11::Initialize(void* hWnd, int screenWidth, int screenHeight)
	{
 		HRESULT hr = S_OK;

		_hWnd = static_cast<HWND>(hWnd);
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		// device와 deviceContext 생성.
		UINT _createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)				// 디버그모드 빌드에서 디버그 계층을 활성화 하기위함!
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;		// 이 플래그를 지정하면 Direct3D는 VC++ 출력 창에 디버그 메시지를 보냄!
#endif

		D3D_FEATURE_LEVEL _levels[7] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1 };

		HR(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			_createDeviceFlags,
			_levels,
			ARRAYSIZE(_levels),
			D3D11_SDK_VERSION,
			_device.GetAddressOf(),
			&_featureLevel,
			_deviceContext.GetAddressOf()));

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
		HR(_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_m4xMsaaQuality));
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
		dxgiDevice.Reset();

		if (SUCCEEDED(hr))
		{
			adapter->GetParent(IID_PPV_ARGS(&factory));

			hr = factory->CreateSwapChain(
				_device.Get(),
				&swapChainDesc,
				&_swapChain
			);
		}

		adapter->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"RocketDX11Adapter") - 1, L"RocketDX11Adapter");
		factory->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"RocketDX11Factory") - 1, L"RocketDX11Factory");

		adapter.Reset();
		factory.Reset();

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

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// 깊이-스텐실 뷰의 description을 작성합니다.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR(_device->CreateDepthStencilView(_depthStencilBuffer.Get(), &depthStencilViewDesc, &_depthStencilView));
		//HR(_device->CreateDepthStencilView(_depthStencilBuffer.Get(), NULL, &_depthStencilView));

		CreateDepthStencilStates();

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

		/// DX 초기화 끝났으니 Manager들 초기화해준다.
		_resourceManager.Initialize(_device.Get(), _deviceContext.Get());
		_objectManager.Initialize(_device.Get());

		/// SpriteBatch, LineBatch, BasicEffect 초기화
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

		_deviceContext->OMSetDepthStencilState(_defaultDepthStencilState.Get(), 0);
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

		_deviceContext->OMSetDepthStencilState(_defaultDepthStencilState.Get(), 0);
		////Blend State Set.
		_deviceContext->OMSetBlendState(_defaultBlendState.Get(), nullptr, 0xFF);

		return;
	}

	void RocketDX11::RenderMesh()
	{
		Camera* mainCam = Camera::GetMainCamera();

		// TODO : 전체 리스트에 있는 것들을 그리는 것이 아니라 현재 씬만 그려야 한다..
		for (auto meshRenderer : _objectManager.GetStaticModelRenderers())
		{
			meshRenderer->Render(_deviceContext.Get(), mainCam->GetViewMatrix(), mainCam->GetProjectionMatrix());
		}

		for (auto dynamicModelRenderer : _objectManager.GetDynamicModelRenderers())
		{
			dynamicModelRenderer->Render(_deviceContext.Get(), mainCam->GetViewMatrix(), mainCam->GetProjectionMatrix());
		}
	}

	void RocketDX11::RenderText()
	{
		_spriteBatch->Begin();
		for (auto textRenderer : _objectManager.GetTextList())
		{
			textRenderer->Render(_spriteBatch);
		}
		_spriteBatch->End();
	}

	void RocketDX11::RenderTexture()
	{
		_spriteBatch->Begin();
		// 이미지(UI)를 그리기 위한 함수
		for (auto imageRenderer : _objectManager.GetImageList())
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
		_isDebugMode = isDebug;
	}

	void RocketDX11::Update(float deltaTime, int fps /*= 0*/)
	{
		_deltaTime = deltaTime;

		Camera::GetMainCamera()->UpdateViewMatrix();
		Camera::GetMainCamera()->UpdateProjectionMatrix();
		UpdateAnimation(deltaTime);
		
		_objectManager._fpsText->SetText(
			"deltaTime : " + std::to_string(_deltaTime)
			+ "\n" + "fps : " + std::to_string(fps)
		);
	}

	void RocketDX11::OnResize(int _width, int _height)
	{

	}

	void RocketDX11::Render()
	{
		BeginRender(0.0f, 0.0f, 0.0f, 1.0f);

		RenderHelperObject();
		RenderMesh();

		RenderCubeMap();

		RenderLine();
		RenderText();
		RenderTexture();

		if (_isDebugMode)
		{
			RenderDebug();
		}

		EndRender();
	}

	void RocketDX11::Finalize()
	{
		_objectManager.Finalize();
		_resourceManager.Finalize();

		delete _spriteBatch;
		delete _lineBatch;		
		_basicEffect.reset();

		// TODO : 여기서 Release를 먼저 해줬더니 아래에서 Reset 하면서 한번 더 지워서 RefCount가 -1이 되는 녀석이 하나 있다.. 뭐하는친구일까?
// 
// 		if (GetRefCount(_device) > 1)
// 		{
// 			_device->Release();
// 		}
// 
// 		if (GetRefCount(_deviceContext) > 1)
// 		{
// 			_deviceContext->Release();
// 		}
// 
// 		if (GetRefCount(_swapChain) > 1)
// 		{
// 			_swapChain->Release();
// 		}
// 
// 		if (GetRefCount(_backBuffer) > 1)
// 		{
// 			_backBuffer->Release();
// 		}
// 
// 		if (GetRefCount(_renderTargetView) > 1)
// 		{
// 			_renderTargetView->Release();
// 		}
// 
// 		if (GetRefCount(_depthStencilBuffer) > 1)
// 		{
// 			_depthStencilBuffer->Release();
// 		}
// 
// 		if (GetRefCount(_depthStencilView) > 1)
// 		{
// 			_depthStencilView->Release();
// 		}
// 
// 		if (GetRefCount(_defaultDepthStencilState) > 1)
// 		{
// 			_defaultDepthStencilState->Release();
// 		}
// 
// 		if (GetRefCount(_cubeMapDepthStencilState) > 1)
// 		{
// 			_cubeMapDepthStencilState->Release();
// 		}
// 
// 		if (GetRefCount(_defaultBlendState) > 1)
// 		{
// 			_defaultBlendState->Release();
// 		}
// 
// 		if (GetRefCount(_lineInputLayout) > 1)
// 		{
// 			_lineInputLayout->Release();
// 		}


		_device->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETdevice") - 1, L"ROCKETdevice");
		_deviceContext->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETdeviceContext") - 1, L"ROCKETdeviceContext");
		_swapChain->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETswapChain") - 1, L"ROCKETswapChain");
		_backBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETbackBuffer") - 1, L"ROCKETbackBuffer");
		_renderTargetView->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETrenderTargetView") - 1, L"ROCKETrenderTargetView");
		_depthStencilBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETdepthStencilBuffer") - 1, L"ROCKETdepthStencilBuffer");
		_depthStencilView->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETdepthStencilView") - 1, L"ROCKETdepthStencilView");
		_defaultDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETdefaultDepthStencilState") - 1, L"ROCKETdefaultDepthStencilState");
		_cubeMapDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETcubeMapDepthStencilState") - 1, L"ROCKETcubeMapDepthStencilState");
		_defaultBlendState->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETdefaultBlendState") - 1, L"ROCKETdefaultBlendState");
		_lineInputLayout->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"ROCKETlineInputLayout") - 1, L"ROCKETlineInputLayout");

		_device.Reset();
		_deviceContext.Reset();
		_swapChain.Reset();
		_backBuffer.Reset();
		_renderTargetView.Reset();
		_depthStencilBuffer.Reset();
		_depthStencilView.Reset();
		_defaultDepthStencilState.Reset();
		_cubeMapDepthStencilState.Reset();
		_defaultBlendState.Reset();
		_lineInputLayout.Reset();
	}

	void RocketDX11::RenderHelperObject()
	{
		auto cam = Camera::GetMainCamera();

		_objectManager._grid->Update(DirectX::XMMatrixIdentity(), cam->GetViewMatrix(), cam->GetProjectionMatrix());
		_objectManager._grid->Render(_deviceContext.Get());
		_objectManager._axis->Update(DirectX::XMMatrixIdentity(), cam->GetViewMatrix(), cam->GetProjectionMatrix());
		_objectManager._axis->Render(_deviceContext.Get());
	}

	void RocketDX11::RenderLine()
	{
		_basicEffect->SetWorld(DirectX::XMMatrixIdentity());
		_basicEffect->SetView(Camera::GetMainCamera()->GetViewMatrix());
		_basicEffect->SetProjection(Camera::GetMainCamera()->GetProjectionMatrix());

		_basicEffect->Apply(_deviceContext.Get());

		_deviceContext->IASetInputLayout(_lineInputLayout.Get());

		_lineBatch->Begin();

		if (_objectManager.GetLineRenderer())
		{
			for (const auto& line : _objectManager.GetLineRenderer()->GetLines())
			{
				_lineBatch->DrawLine(DirectX::VertexPositionColor(line.startPos, line.color), DirectX::VertexPositionColor(line.endPos, line.color));
			}
		}
		_lineBatch->End();
		
		if (_objectManager.GetLineRenderer())
		{
			_objectManager.GetLineRenderer()->Flush();
		}
	}

	void RocketDX11::UpdateAnimation(float deltaTime)
	{
		for (auto& dynamicModel : _objectManager.GetDynamicModelRenderers())
		{
			dynamicModel->UpdateAnimation(deltaTime);
		}
	}

	void RocketDX11::CreateDepthStencilStates()
	{
		/// Create Default DepthStencilState
		D3D11_DEPTH_STENCIL_DESC defaultDepthStencilDesc;
		//CD3D11_DEPTH_STENCIL_DESC depthStencilDesc();

		// 스텐실 상태의 description을 초기화합니다.
		ZeroMemory(&defaultDepthStencilDesc, sizeof(defaultDepthStencilDesc));

		// 스텐실 상태의 description을 작성합니다.
		defaultDepthStencilDesc.DepthEnable = true;
		defaultDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		defaultDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		defaultDepthStencilDesc.StencilEnable = true;
		defaultDepthStencilDesc.StencilReadMask = 0xFF;
		defaultDepthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		defaultDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		defaultDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		defaultDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		defaultDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		defaultDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		defaultDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		defaultDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		defaultDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&defaultDepthStencilDesc, &_defaultDepthStencilState));

		/// Create CubeMapDepthStencilState
		D3D11_DEPTH_STENCIL_DESC cubeMapDepthStencilDesc;
		ZeroMemory(&cubeMapDepthStencilDesc, sizeof(cubeMapDepthStencilDesc));

		cubeMapDepthStencilDesc.DepthEnable = true;
		cubeMapDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		cubeMapDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		cubeMapDepthStencilDesc.StencilEnable = true;
		cubeMapDepthStencilDesc.StencilReadMask = 0xFF;
		cubeMapDepthStencilDesc.StencilWriteMask = 0xFF;

		cubeMapDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		cubeMapDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		cubeMapDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		cubeMapDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		cubeMapDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		cubeMapDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		cubeMapDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		cubeMapDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device->CreateDepthStencilState(&cubeMapDepthStencilDesc, &_cubeMapDepthStencilState));
	}

	void RocketDX11::RenderCubeMap()
	{
		_deviceContext->OMSetDepthStencilState(_cubeMapDepthStencilState.Get(), 0);
		_objectManager.GetDefaultCubeMap()->Render(_deviceContext.Get());
		_deviceContext->OMSetDepthStencilState(_defaultDepthStencilState.Get(), 0);
	}

	void RocketDX11::RenderDebug()
	{
		_spriteBatch->Begin();
		_objectManager._fpsText->Render(_spriteBatch);
		_spriteBatch->End();
	}

}
