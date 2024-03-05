#pragma once

#include "IBuilder.h"
#include <d3d11_2.h>
#pragma comment(lib, "d3d11.lib")

/// <summary>
/// DX11 device 빌더 클래스
/// 
/// 23.06.23 강석원 인재원
/// </summary>
namespace Rocket::Core
{
	class DeviceBuilderDX11 final : public IBuilder
	{
	public:
		// 생성자에서 기본값들로 넣어준다.
		DeviceBuilderDX11();

		void SetAdapter(IDXGIAdapter* adapter);
		void SetDriverType(D3D_DRIVER_TYPE driverType);
		void SetDevice(ID3D11Device** device);
		void SetLevelHolder(D3D_FEATURE_LEVEL* levelHolder);
		void SetDeviceContext(ID3D11DeviceContext** deviceContext);

		// D3D11CreateDevice() 를 호출하여 실행한다.
		// device, featurelevel, deviceContext가 설정되어있지 않으면 S_FALSE를 반환한다.
		long Build() override;

	private:
		IDXGIAdapter* _adapter;
		D3D_DRIVER_TYPE _driverType;
		HMODULE _software;	//소프트웨어 래스터라이저를 구현하는 DLL에 대한 핸들입니다. DriverType이 D3D_DRIVER_TYPE_SOFTWARE경우 소프트웨어는 NULL이 아니어야 합니다. LoadLibrary, LoadLibraryEx 또는 GetModuleHandle을 호출하여 핸들을 가져옵니다.
		UINT _createDeviceFlags;
		D3D_FEATURE_LEVEL _levels[7];
		ID3D11Device** _device;
		D3D_FEATURE_LEVEL* _featureLevel;
		ID3D11DeviceContext** _deviceContext;	// immediateContext
	};

}
