#pragma once
#include <DirectXMath.h>
#include "Light.h"


namespace flt
{
	/// <summary>
	/// 픽셀 쉐이더에서 사용할 상수 버퍼에 들어갈 데이터 구조체
	/// </summary>
	struct PSConstantBuffer
	{
		DirectionalLight directionalLight;
		PointLight pointLight;
		SpotLight spotLight;
		DirectX::XMFLOAT3 eyePosition;
		float padding;
	};
}
