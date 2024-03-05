#pragma once

#include "IBuilder.h"
#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

/// <summary>
/// DX11 device ���� Ŭ����
/// 
/// 23.06.23 ������ �����
/// </summary>
namespace Rocket::Core
{
	class DeviceBuilderDX11 final : public IBuilder
	{
	public:
		// �����ڿ��� �⺻����� �־��ش�.
		DeviceBuilderDX11();

		void SetAdapter(IDXGIAdapter* adapter);
		void SetDriverType(D3D_DRIVER_TYPE driverType);
		void SetDevice(ID3D11Device** device);
		void SetLevelHolder(D3D_FEATURE_LEVEL* levelHolder);
		void SetDeviceContext(ID3D11DeviceContext** deviceContext);

		// D3D11CreateDevice() �� ȣ���Ͽ� �����Ѵ�.
		// device, featurelevel, deviceContext�� �����Ǿ����� ������ S_FALSE�� ��ȯ�Ѵ�.
		long Build() override;

	private:
		IDXGIAdapter* _adapter;
		D3D_DRIVER_TYPE _driverType;
		HMODULE _software;	//����Ʈ���� �����Ͷ������� �����ϴ� DLL�� ���� �ڵ��Դϴ�. DriverType�� D3D_DRIVER_TYPE_SOFTWARE��� ����Ʈ����� NULL�� �ƴϾ�� �մϴ�. LoadLibrary, LoadLibraryEx �Ǵ� GetModuleHandle�� ȣ���Ͽ� �ڵ��� �����ɴϴ�.
		UINT _createDeviceFlags;
		D3D_FEATURE_LEVEL _levels[7];
		ID3D11Device** _device;
		D3D_FEATURE_LEVEL* _featureLevel;
		ID3D11DeviceContext** _deviceContext;	// immediateContext
	};

}
