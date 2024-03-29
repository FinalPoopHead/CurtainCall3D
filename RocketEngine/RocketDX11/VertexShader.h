#pragma once
#include <wrl.h>
#include <vector>

#include "ShaderBase.h"
#include "VertexStruct.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class VertexShader : public ShaderBase
	{
	public:
		VertexShader();
		~VertexShader();

	public:
		virtual void Initialize(ID3D11Device* device, const std::wstring& path) override;

	public:
		ID3D11VertexShader* GetVertexShader() const;
		ID3D11Buffer* GetConstantBuffer(int registerSlot) const;
		ID3D11Buffer** GetAddressOfConstantBuffer(int registerSlot);
		ID3D11InputLayout* GetInputLayout() const;


	public:
		void SetVertexType(eVertexType type);
		eVertexType GetVertexType() const;

	private:
		void CreateAndReflectShader(ID3D11Device* device, const std::wstring& path);
		void ReflectShader(ID3D11Device* device, const std::wstring& path);		// 안쓰는 함수.

	private:
		ComPtr<ID3D11VertexShader> _vertexShader;
		std::vector<ComPtr<ID3D11Buffer>> _constantBuffers;
		ComPtr<ID3D11InputLayout> _inputLayout;
		eVertexType _vertexType;
		unsigned int _numElements;
	};
}
