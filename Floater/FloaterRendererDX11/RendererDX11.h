#pragma once
#include "../FloaterRendererCommon/include/IRenderer.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"
#include "CommonMath.h"
#include "DX11Node.h"
#include "DX11Entity.h"
#include "../FloaterUtil/include/RBTree.h"

#include <windows.h>
#include <wrl/client.h>

#include <d3d11_4.h>
#include <dxgi1_6.h>

#include <vector>
#include <string>


namespace flt
{
	template<typename T>
	using comptr = Microsoft::WRL::ComPtr<T>;

	class RendererDX11 : public IRenderer
	{
#pragma region Nested
	private:
		struct TextureRenderTarget
		{
			comptr<ID3D11Texture2D> texture;
			comptr<ID3D11RenderTargetView> rtv;
			comptr<ID3D11ShaderResourceView> srv;
		};

		enum GBufferType
		{
			GBUFFER_DEPTH,
			GBUFFER_NORMAL,
			GBUFFER_ALBEDO,
			GBUFFER_SPECULAR,
			GBUFFER_EMISSIVE,
			GBUFFER_COUNT
		};

#pragma endregion
	public:
		RendererDX11();
		~RendererDX11();

		bool Initialize(HWND hwnd, HWND debugHWnd = NULL);
		bool Finalize();

		virtual bool Render(float deltaTime);
		virtual HOBJECT RegisterObject(RendererObject& renderable);
		virtual bool DeregisterObject(HOBJECT renderable);

		virtual bool Test() override;
	public:
		bool Resize(unsigned __int32 windowWidth, unsigned __int32 windowHeight);

	private:
		bool InitDisplayInfo();
		bool InitGPUInfo();
		bool OnResize();

		bool SetVsConstantBuffer(ID3D11Buffer* vsConstantBuffer, void* pData, size_t dataSize, UINT slot);

		void SetDX11Node(DX11Node* dxNode, RawNode& node);

		// 테스트 private 함수들
	private:
		Resource<DX11Mesh>* CreateBox();
		Resource<DX11Mesh>* CreatescreenQuad();

	private:
		HWND _hwnd;

		// 엔진 상태등에 관련된 변수들
		bool _isRunRenderEngine;
		bool _useVsync;
		UINT _monitorIndex = 0;
		UINT _refreshRatesIndex = 1;

		unsigned __int32 _windowWidth = 1280;
		unsigned __int32 _windowHeight = 720;

		// 모니터 관련 변수들
		unsigned __int32 _displayWidth = 0;
		unsigned __int32 _displayHeight = 0;
		std::vector<std::vector<DXGI_RATIONAL>> _displayRefreshRates;

		// d3d11 관련 해제해야하는 변수들
		comptr<ID3D11Device> _device;
		comptr<ID3D11DeviceContext> _immediateContext;
		comptr<IDXGIFactory2> _dxgiFactory;
		std::vector<comptr<IDXGIAdapter1>> _adapters;
		std::vector<comptr<IDXGIOutput1>> _outputs;
		comptr<IDXGISwapChain1> _swapChain;
		comptr<ID3D11Texture2D> _backBuffer;
		comptr<ID3D11Texture2D> _depthStencil;
		comptr<ID3D11RenderTargetView> _renderTargetView;
		comptr<ID3D11DepthStencilView> _depthStencilView;
		comptr<ID3D11RasterizerState> _rasterizerState;

		// 디퍼드에 사용할 변수들
		TextureRenderTarget _gBuffer[GBUFFER_COUNT];
		DX11Entity _screenQuad;

		// 그리기 위한 오브젝트
		std::vector<DX11Node*> _renderableObjects;
		std::vector<Camera*> _cameras;

		// 디버그용 변수
		//HWND _debugHWnd;
		//bool _isDebugMode;
		//comptr<IDXGISwapChain1> _debugSwapChain;
		DX11Entity _grids[2];
	};
}


