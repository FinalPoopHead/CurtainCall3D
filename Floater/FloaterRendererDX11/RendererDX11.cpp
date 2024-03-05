#include "RendererDX11.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"
#include "DX11VSConstantBuffer.h"
#include "DX11Mesh.h"
#include <DirectXColors.h>
#include <new>

#if defined(DEBUG) || defined(_DEBUG)
#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")
#endif


#pragma region testIncludes
#include "TestBuilder.h"
#pragma endregion

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")


flt::RendererDX11::RendererDX11() :
	_hwnd(NULL),
	_isRunRenderEngine(false),
	_useVsync(false),
	_monitorIndex(0),
	_refreshRatesIndex(1),
	_windowWidth(1280),
	_windowHeight(720),
	_displayWidth(0),
	_displayHeight(0),
	_displayRefreshRates(),
	_device(),
	_immediateContext(),
	_dxgiFactory(),
	_adapters(),
	_outputs(),
	_swapChain(),
	_backBuffer(),
	_depthStencil(),
	_renderTargetView(),
	_depthStencilView(),
	_rasterizerState(),
	_gBuffer{},
	_screenQuad(),
	_renderableObjects(),
	_cameras(),
	_grids{}
	//_debugHWnd(NULL),
	//_isDebugMode(false)
{

}

flt::RendererDX11::~RendererDX11()
{

}

bool flt::RendererDX11::Initialize(HWND hwnd, HWND debugHWnd /*= NULL*/)
{
	_hwnd = hwnd;

	D3D_FEATURE_LEVEL maxSupportedFeatureLevel = D3D_FEATURE_LEVEL_9_1;

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG) || defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&_device,
		&maxSupportedFeatureLevel,
		&_immediateContext
	);

	if (result != S_OK)
	{
		ASSERT(false, "Device 생성 실패");
		return false;
	}

	UINT numQualityLevels = 0;
	result = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);

	if (result != S_OK)
	{
		ASSERT(false, "멀티 샘플 품질레벨 체크 실패");
		return false;
	}

	result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(IDXGIFactory2), (void**)(&_dxgiFactory));

	if (result != S_OK)
	{
		ASSERT(false, "DXGI Factory 생성 실패");
		return false;
	}

	if (!InitDisplayInfo())
	{
		ASSERT(false, "모니터 모드 초기화 실패");
		return false;
	}

	if (!InitGPUInfo())
	{
		ASSERT(false, "GPU 정보 초기화 실패");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC1 desc = { 0, };

	desc.Width = 0; // CreateSwapChainForHwnd 로 호출 시 0으로 세팅하면 해당 hwnd에서 런타임에 출력창에서 너비를 가져옵니다.
	desc.Height = 0; // CreateSwapChainForHwnd 로 호출 시 0으로 세팅하면 해당 hwnd에서 런타임에 출력창에서 높이를 가져옵니다.
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Stereo = FALSE;
	desc.SampleDesc = { 1, 0 };
	desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = { };
	fullScreenDesc.RefreshRate = { 0, 0 };
	fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;
	fullScreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	fullScreenDesc.Windowed = true;

	result = _dxgiFactory->CreateSwapChainForHwnd(_device.Get(), _hwnd, &desc, &fullScreenDesc, NULL, &_swapChain);
	if (result != S_OK)
	{
		ASSERT(false, "스왑체인 생성 실패");
		return false;
	}

	OnResize();
	//if (!Resize(1280, 720))
	//{
	//	ASSERT(false, "화면 사이즈 변경 실패");
	//	return false;
	//}

	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = _device->CreateRasterizerState(&rasterizerDesc, &_rasterizerState);
	if (result != S_OK)
	{
		ASSERT(false, "RasterizerState 생성 실패");
		return false;
	}

	_immediateContext->RSSetState(_rasterizerState.Get());

	//if (debugHWnd != NULL)
	//{
	//	_debugHWnd = debugHWnd;

	//	result = _dxgiFactory->CreateSwapChainForHwnd(_device.Get(), _debugHWnd, &desc, &fullScreenDesc, NULL, &_debugSwapChain);
	//	if (result != S_OK)
	//	{
	//		ASSERT(false, "스왑체인 생성 실패");
	//		return false;
	//	}

	//	_isDebugMode = true;
	//}

	_screenQuad.node = new(std::nothrow) DX11Node(_screenQuad.transform, _screenQuad.isDraw);
	ASSERT(_screenQuad.node, "스크린 쿼드 노드 생성 실패");

	_screenQuad.node->name = L"screenQuad";
	DX11ScreedQuadBuilder screenQuadBuilder;
	screenQuadBuilder.pDevice = _device.Get();
	screenQuadBuilder.pImmediateContext = _immediateContext.Get();

	_screenQuad.node->meshes.resize(1);
	_screenQuad.node->meshes[0].Set(screenQuadBuilder);
	ASSERT(_screenQuad.node->meshes[0].Get(), "메쉬 생성 실패");


	int gridCounts[2] = { 161, 801 };
	for (int i = 0; i < 2; ++i)
	{
		_grids[i].node = new(std::nothrow) DX11Node(_grids[i].transform, _grids[i].isDraw);
		ASSERT(_grids[i].node, "그리드 노드 생성 실패");

		_grids[i].node->name = L"grid";
		_grids[i].node->meshes.resize(1);
		DX11GridMeshBuilder gridBuilder;
		gridBuilder.pDevice = _device.Get();
		gridBuilder.pImmediateContext = _immediateContext.Get();
		gridBuilder.gridCount = gridCounts[i];
		std::wstring gridName = L"flt::GridMeshBuilder";
		gridName += std::to_wstring(i);
		gridBuilder.key = gridName;
		_grids[i].node->meshes[0].Set(gridBuilder);
		ASSERT(_grids[i].node->meshes[0].Get(), "메쉬 생성 실패");
	}

	_isRunRenderEngine = true;
	return true;
}

bool flt::RendererDX11::Finalize()
{
	_device = nullptr;
	_immediateContext = nullptr;
	_dxgiFactory = nullptr;
	_swapChain = nullptr;
	_backBuffer = nullptr;
	_depthStencil = nullptr;
	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_rasterizerState = nullptr;

	for (auto& adapter : _adapters)
	{
		adapter = nullptr;
	}
	_adapters.clear();

	for (auto& output : _outputs)
	{
		output = nullptr;
	}
	_outputs.clear();

	_isRunRenderEngine = false;

	delete _screenQuad.node;
	_screenQuad.node = nullptr;
	delete _grids[0].node;
	_grids[0].node = nullptr;
	delete _grids[1].node;
	_grids[1].node = nullptr;

	// 들고 있는 노드들 삭제.
	for (auto& node : _renderableObjects)
	{
		delete node;
		node = nullptr;
	}
	_renderableObjects.clear();
	_cameras.clear();

	//#if defined(DEBUG) || defined(_DEBUG)
	//	IDXGIDebug1* dxgiDebug;
	//	DXGIGetDebugInterface1(0, __uuidof(IDXGIDebug1), (void**)&dxgiDebug);
	//	dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
	//	dxgiDebug->Release();
	//#endif
	return true;
}

bool flt::RendererDX11::Render(float deltaTime)
{
	// 디퍼드 시작 일단 멀티 렌더타겟.
	ID3D11RenderTargetView* rtvs[GBUFFER_COUNT] =
	{
		_gBuffer[GBUFFER_DEPTH].rtv.Get(),
		_gBuffer[GBUFFER_NORMAL].rtv.Get(),
		_gBuffer[GBUFFER_ALBEDO].rtv.Get(),
		_gBuffer[GBUFFER_SPECULAR].rtv.Get(),
		_gBuffer[GBUFFER_EMISSIVE].rtv.Get(),
	};

	for (int i = 0; i < GBUFFER_COUNT; ++i)
	{
		_immediateContext->ClearRenderTargetView(rtvs[i], DirectX::Colors::Yellow);
	}
	_immediateContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	_immediateContext->OMSetRenderTargets(GBUFFER_COUNT, rtvs, _depthStencilView.Get());

	// 각 mesh별로 들어가야하지만 일단 여기서 만들자.
	// 매번 만들지 않도록 수정해야함.
	D3D11_RASTERIZER_DESC rasterizerDesc = { };
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	_device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	_immediateContext->RSSetState(rasterizerState.Get());

	{
		//디버그용 그리드 그리기

		auto& camera = _cameras[0];
		Matrix4f viewMatrix = camera->GetViewMatrix();
		Matrix4f projMatrix = camera->GetProjectionMatrix();

		const Transform* cameraTransform = camera->GetTransform();
		Vector4f cameraPosition = cameraTransform->GetLocalPosition();

		// 투명으로 그리기위한 임시 블렌딩 상태 생성
		D3D11_BLEND_DESC blendDesc = { };
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ID3D11BlendState* blendState;
		auto ret = _device->CreateBlendState(&blendDesc, &blendState);
		ASSERT(ret == S_OK, "블렌드 상태 생성 실패");
		float blend[4] = { 1.0f,1.0f,1.0f, 1.0f };
		_immediateContext->OMSetBlendState(blendState, blend, 0xffffffff);
		blendState->Release();

		// 그리드 스케일은 카메라 높이에 따라서 1, 10, 100, 1000, 10000 중 하나.
		float gridScale = 1.0f;

		if (cameraPosition.y > 10000.0f)
		{
			gridScale = 10000.0f;
		}
		else if (cameraPosition.y > 1000.0f)
		{
			gridScale = 1000.0f;
		}
		else if (cameraPosition.y > 100.0f)
		{
			gridScale = 100.0f;
		}
		else if (cameraPosition.y > 10.0f)
		{
			gridScale = 10.0f;
		}
		float secondGridScale = (float)((int)gridScale / 10);
		float heightOpacitys[2] = { 1.0f, 0.0f };

		if (secondGridScale > 0.0f)
		{
			// 0~5 일경우는 0, 5~10일경우 0~1 사이로 변경.
			heightOpacitys[1] = ((cameraPosition.y / gridScale) - 5.0f) * 2.0f;
			heightOpacitys[1] < 0.0f ? heightOpacitys[1] = 0.0f : heightOpacitys[1];
			heightOpacitys[1] /= 10.0f;
			heightOpacitys[1] = 1.0f - heightOpacitys[1];
			ASSERT(heightOpacitys[1] >= 0.0f && heightOpacitys[1] <= 1.0f, "높이 투명도 오류");
		}


		_grids[0].transform.SetScale(gridScale, gridScale, gridScale);
		_grids[1].transform.SetScale(secondGridScale, secondGridScale, secondGridScale);

		// 그리드의 위치는 카메라의 위치에서 가장 가까운 정수값위치 * 그리드 스케일
		_grids[0].transform.SetPosition(
			(int)(cameraPosition.x / gridScale) * gridScale,
			0.0f,
			(int)(cameraPosition.z / gridScale) * gridScale
		);

		_grids[1].transform.SetPosition(
			(int)(cameraPosition.x / secondGridScale) * secondGridScale,
			0.0f,
			(int)(cameraPosition.z / secondGridScale) * secondGridScale
		);


		D3D11_RASTERIZER_DESC wireFrameRasterizerDesc = {};
		wireFrameRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireFrameRasterizerDesc.CullMode = D3D11_CULL_NONE;
		wireFrameRasterizerDesc.FrontCounterClockwise = false;
		wireFrameRasterizerDesc.DepthBias = 0;
		wireFrameRasterizerDesc.DepthBiasClamp = 0.0f;
		wireFrameRasterizerDesc.SlopeScaledDepthBias = 0.0f;
		wireFrameRasterizerDesc.DepthClipEnable = true;
		wireFrameRasterizerDesc.ScissorEnable = false;
		wireFrameRasterizerDesc.MultisampleEnable = false;
		wireFrameRasterizerDesc.AntialiasedLineEnable = false;

		ID3D11RasterizerState* wireFrameRasterizer;
		_device->CreateRasterizerState(&wireFrameRasterizerDesc, &wireFrameRasterizer);
		_immediateContext->RSSetState(wireFrameRasterizer);
		wireFrameRasterizer->Release();

		int gridCounts[2] = { 161, 801 };
		for (int i = 0; i < 2; ++i)
		{
			DX11Mesh* pGridMesh = _grids[i].node->meshes[0].Get();
			DX11VertexShader* girdVsShader = pGridMesh->vertexShader.Get();
			DX11PixelShader* pixelShader = pGridMesh->pixelShader.Get();
			_immediateContext->IASetInputLayout(girdVsShader->pInputLayout);
			_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			_immediateContext->VSSetShader(girdVsShader->pVertexShader, nullptr, 0);
			_immediateContext->PSSetShader(pixelShader->pPixelShader, nullptr, 0);

			// 상수 버퍼 임시 세팅
			struct {
				unsigned int gridCount;
			} entityInitData;
			entityInitData.gridCount = gridCounts[i];

			struct {
				DirectX::XMFLOAT3 cameraPos;
			}framePerCamera;
			framePerCamera.cameraPos = { cameraPosition.x, cameraPosition.y, cameraPosition.z };

			struct {
				DirectX::XMMATRIX worldTranslate;
				DirectX::XMMATRIX worldViewProj;
				float heightOpacity;
			}framePerEntity;

			framePerEntity.worldTranslate = ConvertXMMatrix(_grids[i].transform.GetTranslateMatrix4f());
			framePerEntity.worldViewProj = ConvertXMMatrix(_grids[i].transform.GetWorldMatrix4f() * viewMatrix * projMatrix);
			framePerEntity.heightOpacity = heightOpacitys[i];

			void* pData[3] = { &entityInitData , &framePerCamera , &framePerEntity };
			girdVsShader->SetConstantBuffer(_immediateContext.Get(), pData, 3);

			_immediateContext->PSSetSamplers(0, 1, &pGridMesh->sampler);

			UINT offset = 0;
			_immediateContext->IASetVertexBuffers(0, 1, &pGridMesh->vertexBuffer, &pGridMesh->singleVertexSize, &offset);
			_immediateContext->IASetIndexBuffer(pGridMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			_immediateContext->DrawIndexed(pGridMesh->indexCount, 0, 0);
		}

		// 원래 상태로 복구
		_immediateContext->RSSetState(NULL);
		_immediateContext->OMSetBlendState(NULL, blend, 0xFFFFFFFF);
	}

	for (auto& camera : _cameras)
	{
		Matrix4f viewMatrix = camera->GetViewMatrix();
		Matrix4f projMatrix = camera->GetProjectionMatrix();

		for (auto& node : _renderableObjects)
		{
			int meshCount = (int)node->meshes.size();

			Matrix4f worldMatrix = node->transform.GetWorldMatrix4f();
			Matrix4f worldViewProjMatrix = worldMatrix * viewMatrix * projMatrix;
			DirectX::XMMATRIX world = ConvertXMMatrix(worldMatrix);
			DirectX::XMMATRIX worldViewProj = ConvertXMMatrix(worldViewProjMatrix);

			for (auto& mesh : node->meshes)
			{
				//if (mesh->pRootBoneTransform)
				//{

				//}

				DX11Mesh* pMesh = mesh.Get();

				DX11VertexShader* vertexShader = pMesh->vertexShader.Get();
				DX11PixelShader* pixelShader = pMesh->pixelShader.Get();

				_immediateContext->IASetInputLayout(vertexShader->pInputLayout);
				_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				_immediateContext->VSSetShader(vertexShader->pVertexShader, nullptr, 0);
				_immediateContext->PSSetShader(pixelShader->pPixelShader, nullptr, 0);

				_immediateContext->PSSetShaderResources(0, pMesh->srvCount, pMesh->srv);

				void* pData = &worldViewProj;
				vertexShader->SetConstantBuffer(_immediateContext.Get(), &pData, 1);

				_immediateContext->PSSetSamplers(0, 1, &pMesh->sampler);

				UINT offset = 0;
				_immediateContext->IASetVertexBuffers(0, 1, &pMesh->vertexBuffer, &pMesh->singleVertexSize, &offset);
				_immediateContext->IASetIndexBuffer(pMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				_immediateContext->DrawIndexed(pMesh->indexCount, 0, 0);
			}
		}
	}

	// 마찬가지로 BlendState도 매번 만들지 않도록 수정해야함.
	D3D11_BLEND_DESC blendDesc = { };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	_device->CreateBlendState(&blendDesc, &blendState);
	float blend[4] = { 1,1,1, 1 };

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
	_device->CreateDepthStencilState(&depthStencilDesc, &depthState);

	_immediateContext->OMSetBlendState(blendState.Get(), blend, 0xFFFFFFFF);
	_immediateContext->OMSetDepthStencilState(depthState.Get(), 0);

	// 빛 연산에 필요한 텍스쳐 픽셀쉐이더에 세팅.
	/*_immediateContext->PSSetShaderResources(GBUFFER_DEPTH, 1, _gBuffer[GBUFFER_DEPTH].srv.GetAddressOf());
	_immediateContext->PSSetShaderResources(GBUFFER_NORMAL, 1, _gBuffer[GBUFFER_NORMAL].srv.GetAddressOf());
	_immediateContext->PSSetShaderResources(GBUFFER_ALBEDO, 1, _gBuffer[GBUFFER_ALBEDO].srv.GetAddressOf());*/

	// 빛 연산 로직 구현 필요 TODO

	// 최종 연산 결과 화면에 출력
	{
		_immediateContext->ClearRenderTargetView(_renderTargetView.Get(), DirectX::Colors::Black);
		_immediateContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		_immediateContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
		_immediateContext->RSSetState(rasterizerState.Get());

		DX11Mesh* pMesh = _screenQuad.node->meshes[0].Get();
		DX11VertexShader* vertexShader = pMesh->vertexShader.Get();
		DX11PixelShader* pixelShader = pMesh->pixelShader.Get();

		_immediateContext->IASetInputLayout(vertexShader->pInputLayout);
		_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		_immediateContext->VSSetShader(vertexShader->pVertexShader, nullptr, 0);
		_immediateContext->PSSetShader(pixelShader->pPixelShader, nullptr, 0);

		for (int i = 0; i < GBUFFER_COUNT; ++i)
		{
			_immediateContext->PSSetShaderResources(i, 1, _gBuffer[i].srv.GetAddressOf());
		}

		// 이 크기에 맞춰서 가로세로 x y 각각 0~1 사이의 값을 통해 조절
		VSBackBuffer vsBackBuffer{ 1.f, 1.f, 0.5f, 0.5f };
		void* pData = &vsBackBuffer;
		vertexShader->SetConstantBuffer(_immediateContext.Get(), &pData, 1);

		_immediateContext->PSSetSamplers(0, 1, &pMesh->sampler);

		UINT offset = 0;
		_immediateContext->IASetVertexBuffers(0, 1, &pMesh->vertexBuffer, &pMesh->singleVertexSize, &offset);
		_immediateContext->IASetIndexBuffer(pMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		_immediateContext->DrawIndexed(pMesh->indexCount, 0, 0);

		// 디버깅 정보를 띄울경우에 출력
		{

		}

		// 입력에 사용한 텍스쳐 정리
		ID3D11ShaderResourceView* nullSRV[GBUFFER_COUNT] = { NULL, };
		_immediateContext->PSSetShaderResources(0, GBUFFER_COUNT, nullSRV);
	}

	_immediateContext->RSSetState(NULL);
	_immediateContext->OMSetBlendState(NULL, blend, 0xFFFFFFFF);

	// 수직동기화 여부에 따라서 present
	if (_useVsync)
	{
		_swapChain->Present(1, 0);
	}
	else
	{
		_swapChain->Present(0, 0);
	}

	return true;
}

flt::HOBJECT flt::RendererDX11::RegisterObject(RendererObject& renderable)
{
	DX11Node* node = new DX11Node(renderable.node.transform, renderable.isDraw);
	if (!node)
	{
		return false;
	}

	if (renderable.name == L"cube")
	{
		DX11CubeMeshBuilder cubeBuilder;
		cubeBuilder.pDevice = _device.Get();
		cubeBuilder.pImmediateContext = _immediateContext.Get();

		node->meshes.resize(1);
		node->meshes[0].Set(cubeBuilder);
		ASSERT(node->meshes[0].Get(), "메쉬 생성 실패");
	}
	else
	{
		SetDX11Node(node, renderable.node);
	}

	_renderableObjects.push_back(node);

	if (renderable.node.camera)
	{
		node->camera = renderable.node.camera;
		_cameras.push_back(node->camera);
	}

	return true;
}

bool flt::RendererDX11::DeregisterObject(HOBJECT renderable)
{
	return false;
}

bool flt::RendererDX11::Test()
{
	test::BuilderTester tester;
	return tester.Test();
}

bool flt::RendererDX11::Resize(unsigned __int32 windowWidth, unsigned __int32 windowHeight)
{
	if (!_swapChain)
	{
		return false;
	}

	DXGI_MODE_DESC desc = { 0, };
	desc.Width = windowWidth;
	desc.Height = windowHeight;
	if (_useVsync)
	{
		// 일단은 임시로 세팅해 놓자.
		desc.RefreshRate.Numerator = _displayRefreshRates[_monitorIndex][_refreshRatesIndex].Numerator;
		desc.RefreshRate.Denominator = _displayRefreshRates[_monitorIndex][_refreshRatesIndex].Denominator;
	}
	else
	{
		desc.RefreshRate.Numerator = 0;
		desc.RefreshRate.Denominator = 1;
	}

	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	_swapChain->ResizeTarget(&desc);

	OnResize();

	_windowWidth = windowHeight;
	_windowHeight = windowHeight;

	_displayWidth = windowHeight;
	_displayHeight = windowHeight;
	return true;
}

bool flt::RendererDX11::InitDisplayInfo()
{
	if (_dxgiFactory == nullptr)
	{
		ASSERT(false, "dxgiFactory 부터 만드세요");
		return false;
	}

	IDXGIAdapter1* adapter = nullptr;
	for (int i = 0; _dxgiFactory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		_adapters.emplace_back(adapter);
		adapter->Release();
	}

	IDXGIOutput1* output = nullptr;
	for (int i = 0; _adapters[0]->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		_outputs.emplace_back(output);
		output->Release();
	}

	_displayRefreshRates.resize(_outputs.size());

	for (int i = 0; i < _outputs.size(); ++i)
	{
		unsigned __int32 displayModeNum = 0;
		HRESULT result = _outputs[i]->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &displayModeNum, NULL);
		if (result != S_OK)
		{
			ASSERT(false, "디스플레이 모드 리스트 갯수 가져오기 실패");
			return false;
		}
		if (displayModeNum == 0)
		{
			ASSERT(false, "디스플레이 모드 리스트 갯수가 0개 입니다.");
			return false;
		}

		DXGI_MODE_DESC1* dxgiModeArr = new(std::nothrow) DXGI_MODE_DESC1[displayModeNum];

		if (dxgiModeArr == nullptr)
		{
			ASSERT(false, "디스플레이 모드 리스트 할당 실패");
			return false;
		}

		result = _outputs[i]->GetDisplayModeList1(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &displayModeNum, dxgiModeArr);
		if (result != S_OK)
		{
			delete[] dxgiModeArr;
			return false;
		}

		for (unsigned int j = 0; j < displayModeNum; ++j)
		{
			if (dxgiModeArr[j].Width == _displayWidth
				&& dxgiModeArr[j].Height == _displayHeight)
			{
				_displayRefreshRates[i].emplace_back(dxgiModeArr[j].RefreshRate);
			}
		}

		delete[] dxgiModeArr;
	}

	return true;
}

bool flt::RendererDX11::InitGPUInfo()
{
	if (_adapters.empty())
	{
		ASSERT(false, "디스플레이 모드를 먼저 초기화 해주세요");
		return false;
	}

	for (int i = 0; i < _adapters.size(); ++i)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		_adapters[i]->GetDesc1(&adapterDesc);

		SIZE_T gpuMemoryMB = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		std::wstring gpuName = adapterDesc.Description;
	}

	return true;
}

bool flt::RendererDX11::OnResize()
{
	if (!_immediateContext)
	{
		return false;
	}

	_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	_renderTargetView = nullptr;
	_depthStencilView = nullptr;
	_depthStencil = nullptr;
	_backBuffer = nullptr;

	HRESULT result = _swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
	if (result != S_OK)
	{
		ASSERT(false, "스왑체인 리사이즈 실패");
		return false;
	}

	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&_backBuffer));

	if (result != S_OK)
	{
		ASSERT(false, "버퍼 가져오기 실패");
		return false;
	}

	result = _device->CreateRenderTargetView(_backBuffer.Get(), nullptr, &_renderTargetView);

	if (result != S_OK)
	{
		ASSERT(false, "렌더 타겟 뷰 생성 실패");
		return false;
	}

	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	_backBuffer->GetDesc(&backBufferDesc);

	// 뎁스 스텐실 뷰 만들기
	CD3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Width = backBufferDesc.Width;
	depthStencilDesc.Height = backBufferDesc.Height;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_device->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencil);

	_device->CreateDepthStencilView(_depthStencil.Get(), nullptr, &_depthStencilView);

	_immediateContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

	// 뷰포트 생성
	D3D11_VIEWPORT viewPort[1]{};
	viewPort[0].TopLeftX = 0.0f;
	viewPort[0].TopLeftY = 0.0f;
	viewPort[0].Width = (FLOAT)backBufferDesc.Width;
	viewPort[0].Height = (FLOAT)backBufferDesc.Height;
	viewPort[0].MinDepth = 0.0f;
	viewPort[0].MaxDepth = 1.0f;

	_immediateContext->RSSetViewports(1, viewPort);

	_displayWidth = backBufferDesc.Width;
	_displayHeight = backBufferDesc.Height;

	// 테스트를 위한 배경 색상 변경
	float bgColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	_immediateContext->ClearRenderTargetView(_renderTargetView.Get(), bgColor);

	//g-buffer또한 다시 만들기
	D3D11_TEXTURE2D_DESC gBufferTextureDesc{};
	gBufferTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gBufferTextureDesc.Width = backBufferDesc.Width;
	gBufferTextureDesc.Height = backBufferDesc.Height;
	gBufferTextureDesc.MipLevels = 1;
	gBufferTextureDesc.ArraySize = 1;
	gBufferTextureDesc.SampleDesc.Count = 1;
	gBufferTextureDesc.SampleDesc.Quality = 0;
	gBufferTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	D3D11_RENDER_TARGET_VIEW_DESC gBufferRenderTargetViewDesc{};
	gBufferRenderTargetViewDesc.Format = gBufferTextureDesc.Format;
	gBufferRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	D3D11_SHADER_RESOURCE_VIEW_DESC gBufferShaderResourceViewDesc{};
	gBufferShaderResourceViewDesc.Format = gBufferTextureDesc.Format;
	gBufferShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	gBufferShaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < GBUFFER_COUNT; ++i)
	{
		result = _device->CreateTexture2D(&gBufferTextureDesc, nullptr, &_gBuffer[i].texture);
		if (result != S_OK)
		{
			ASSERT(false, "G-Buffer 텍스쳐 생성 실패");
			return false;
		}

		result = _device->CreateRenderTargetView(_gBuffer[i].texture.Get(), &gBufferRenderTargetViewDesc, &_gBuffer[i].rtv);
		if (result != S_OK)
		{
			ASSERT(false, "G-Buffer 렌더 타겟 뷰 생성 실패");
			return false;
		}

		result = _device->CreateShaderResourceView(_gBuffer[i].texture.Get(), &gBufferShaderResourceViewDesc, &_gBuffer[i].srv);
		if (result != S_OK)
		{
			ASSERT(false, "G-Buffer 쉐이더 리소스 뷰 생성 실패");
			return false;
		}
	}

	return true;
}

bool flt::RendererDX11::SetVsConstantBuffer(ID3D11Buffer* vsConstantBuffer, void* pData, size_t dataSize, UINT slot)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hResult = _immediateContext->Map(vsConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	if (hResult != S_OK)
	{
		return false;
	}

	memcpy(mappedResource.pData, pData, dataSize);
	_immediateContext->Unmap(vsConstantBuffer, 0);

	_immediateContext->VSSetConstantBuffers(slot, 1, &vsConstantBuffer);

	return true;
}

void flt::RendererDX11::SetDX11Node(DX11Node* dxNode, RawNode& node)
{
	int rawMeshCount = (int)node.meshes.size();
	dxNode->meshes.resize(rawMeshCount);
	for (int i = 0; i < rawMeshCount; ++i)
	{
		DX11MeshBuilder meshBuilder(node.name + std::to_wstring(i));
		meshBuilder.pDevice = _device.Get();
		meshBuilder.vsBuilder = DX11VertexShaderBuilder(L"flt::CubeVS");
		meshBuilder.pImmediateContext = _immediateContext.Get();
		meshBuilder.pRawMesh = node.meshes[i].Get();

		dxNode->meshes[i].Set(meshBuilder);
		ASSERT(dxNode->meshes[i].Get(), "Set Mesh fail");
	}
}

flt::Resource<flt::DX11Mesh>* flt::RendererDX11::CreateBox()
{
	DX11CubeMeshBuilder cubeBuilder;
	cubeBuilder.pDevice = _device.Get();
	cubeBuilder.pImmediateContext = _immediateContext.Get();

	Resource<DX11Mesh>* meshResource = new Resource<DX11Mesh>(cubeBuilder);

	return meshResource;
}

flt::Resource<flt::DX11Mesh>* flt::RendererDX11::CreatescreenQuad()
{
	DX11ScreedQuadBuilder screenQuadBuilder;
	screenQuadBuilder.pDevice = _device.Get();
	screenQuadBuilder.pImmediateContext = _immediateContext.Get();

	Resource<DX11Mesh>* meshResource = new Resource<DX11Mesh>(screenQuadBuilder);

	return meshResource;
}
