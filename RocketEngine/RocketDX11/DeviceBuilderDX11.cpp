#include "DeviceBuilderDX11.h"

namespace Rocket::Core
{
	DeviceBuilderDX11::DeviceBuilderDX11()
		:_adapter(nullptr),
		_driverType(D3D_DRIVER_TYPE_HARDWARE),
		_software(nullptr),
		_createDeviceFlags(),
		_levels{ D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1 },
		_device(),
		_featureLevel(nullptr),
		_deviceContext()

	{

	}

	void DeviceBuilderDX11::SetAdapter(IDXGIAdapter* adapter)
	{
		_adapter = adapter;
	}

	void DeviceBuilderDX11::SetDriverType(D3D_DRIVER_TYPE driverType)
	{
		_driverType = driverType;
		if (_driverType == D3D_DRIVER_TYPE_SOFTWARE)
		{
			// 여기서 뭔가를 해줘야 한다는데 우선 패쓰.
			// 23.06.23 강석원 인재원.
			// 
			// 소프트웨어 래스터라이저를 구현하는 DLL에 대한 핸들입니다.
			// DriverType이 D3D_DRIVER_TYPE_SOFTWARE경우 소프트웨어는 NULL이 아니어야 합니다.
			// LoadLibrary, LoadLibraryEx 또는 GetModuleHandle을 호출하여 핸들을 가져옵니다.
		}
	}

	void DeviceBuilderDX11::SetDevice(ID3D11Device** device)
	{
		_device = device;
	}

	void DeviceBuilderDX11::SetLevelHolder(D3D_FEATURE_LEVEL* levelHolder)
	{
		_featureLevel = levelHolder;
	}

	void DeviceBuilderDX11::SetDeviceContext(ID3D11DeviceContext** deviceContext)
	{
		_deviceContext = deviceContext;
	}

	long DeviceBuilderDX11::Build()
	{
		HRESULT hr = S_OK;

		if (!_device || !_featureLevel || !_deviceContext)
		{
			return S_FALSE;
		}

#if defined(DEBUG) || defined(_DEBUG)				// 디버그모드 빌드에서 디버그 계층을 활성화 하기위함!
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;		// 이 플래그를 지정하면 Direct3D는 VC++ 출력 창에 디버그 메시지를 보냄!
#endif

		hr = D3D11CreateDevice(
			_adapter,
			_driverType,
			_software,
			_createDeviceFlags,
			_levels,
			ARRAYSIZE(_levels),
			D3D11_SDK_VERSION,
			_device,
			_featureLevel,
			_deviceContext);

		if (FAILED(hr))
		{
			// Handle device interface creation failure if it occurs.
			// For example, reduce the feature level requirement, or fail over 
			// to WARP rendering.

			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		}

		return hr;
	}

}
