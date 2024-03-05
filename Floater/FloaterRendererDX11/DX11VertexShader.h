#pragma once
#include <d3d11.h>
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"

namespace flt
{
	class ResourceMgr;

	struct DX11VertexShader
	{
#pragma region Nasted
		enum CBufferSlot
		{
			ENTITY_INIT_DATA = 0,
			FRAME_PER_CAMERA = 1,
			FRAME_PER_ENTITY = 2,
			FRAME_PER_MATERIAL = 3,

		};

#pragma endregion

		DX11VertexShader() : pVertexShader(nullptr), pInputLayout(nullptr), pConstantBuffers() {}

		void Release()
		{
			if (pVertexShader)
			{
				pVertexShader->Release();
			}
			pVertexShader = nullptr;

			if (pInputLayout)
			{
				pInputLayout->Release();
			}
			pInputLayout = nullptr;

			int constantBufferArrSize = (int)pConstantBuffers.size();
			for (int i = 0; i < constantBufferArrSize; ++i)
			{
				pConstantBuffers[i].first->Release();
			}
			pConstantBuffers.clear();
		}

		void SetConstantBuffer(ID3D11DeviceContext* pContext, void** pDataArr, int arrSize);

		ID3D11VertexShader* pVertexShader;
		ID3D11InputLayout* pInputLayout;
		std::vector<std::pair<ID3D11Buffer*, int>> pConstantBuffers;
	};

	template struct Resource<DX11VertexShader>;

	struct DX11VertexShaderBuilder : public IBuilder<DX11VertexShader>
	{
		DX11VertexShaderBuilder() : DX11VertexShaderBuilder(L"") {}
		DX11VertexShaderBuilder(const std::wstring filePath) : IBuilder<DX11VertexShader>(filePath), filePath(filePath), pDevice(nullptr), inputLayoutDesc() {}
		DX11VertexShaderBuilder(const DX11VertexShaderBuilder& other) = delete;
		DX11VertexShaderBuilder(DX11VertexShaderBuilder&& other) noexcept : IBuilder<DX11VertexShader>(std::move(other)), pDevice(other.pDevice), inputLayoutDesc(std::move(other.inputLayoutDesc))
		{
			other.pDevice = nullptr;
		}

		DX11VertexShaderBuilder& operator=(const DX11VertexShaderBuilder& other) = delete;
		DX11VertexShaderBuilder& operator=(DX11VertexShaderBuilder&& other) noexcept
		{
			IBuilder<DX11VertexShader>::operator=(std::move(other));
			pDevice = other.pDevice;
			inputLayoutDesc = other.inputLayoutDesc;
			other.pDevice = nullptr;
			return *this;
		}

		virtual ~DX11VertexShaderBuilder();

		virtual DX11VertexShader* build() const override;

		std::wstring filePath;
		ID3D11Device* pDevice;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		std::vector<int> constantBufferSizes;
	};
}


