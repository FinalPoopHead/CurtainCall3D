#pragma once
#include <string>
#include <d3d11.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class ShaderBase
	{
	public:
		virtual void Initialize(ID3D11Device* device, const std::wstring& path) = 0;
	};
}
