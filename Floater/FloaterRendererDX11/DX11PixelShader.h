#pragma once
#include <d3d11.h>
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"

namespace flt
{
	class ResourceMgr;

	struct DX11PixelShader
	{
		DX11PixelShader() : pPixelShader(nullptr), pConstantBuffer(nullptr) {}
		void Release();

		ID3D11PixelShader* pPixelShader;
		ID3D11Buffer* pConstantBuffer;
	};

	template struct Resource<DX11PixelShader>;

	struct DX11PixelShaderBuilder : public IBuilder<DX11PixelShader>
	{
		DX11PixelShaderBuilder() : DX11PixelShaderBuilder(L"") {}
		DX11PixelShaderBuilder(const std::wstring filePath) : IBuilder<DX11PixelShader>(filePath), filePath(filePath), pDevice(nullptr) {}

		virtual DX11PixelShader* build() const override;

		std::wstring filePath;
		ID3D11Device* pDevice;
	};
}

