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
			// ���⼭ ������ ����� �Ѵٴµ� �켱 �о�.
			// 23.06.23 ������ �����.
			// 
			// ����Ʈ���� �����Ͷ������� �����ϴ� DLL�� ���� �ڵ��Դϴ�.
			// DriverType�� D3D_DRIVER_TYPE_SOFTWARE��� ����Ʈ����� NULL�� �ƴϾ�� �մϴ�.
			// LoadLibrary, LoadLibraryEx �Ǵ� GetModuleHandle�� ȣ���Ͽ� �ڵ��� �����ɴϴ�.
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

#if defined(DEBUG) || defined(_DEBUG)				// ����׸�� ���忡�� ����� ������ Ȱ��ȭ �ϱ�����!
		_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;		// �� �÷��׸� �����ϸ� Direct3D�� VC++ ��� â�� ����� �޽����� ����!
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
