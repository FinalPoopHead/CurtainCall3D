#pragma once
#include <wrl.h>
#include <vector>

#include "ShaderBase.h"

namespace Rocket::Core
{
	class PixelShader : public ShaderBase
	{
	public:
		PixelShader();
		~PixelShader();

	public:
		virtual void Initialize(ID3D11Device* device, const std::wstring& path) override;

	public:
		ID3D11PixelShader* GetPixelShader() const;
		ID3D11Buffer* GetConstantBuffer(int registerSlot) const;
		ID3D11Buffer** GetAddressOfConstantBuffer(int registerSlot);

	private:
		void CreateAndReflectShader(ID3D11Device* device, const std::wstring& path);


	private:
		ComPtr<ID3D11PixelShader> _pixelShader;
		std::vector<ComPtr<ID3D11Buffer>> _constantBuffer;
	};
}
