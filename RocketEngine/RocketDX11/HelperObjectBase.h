#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

#include "MathHeader.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class VertexShader;
	class PixelShader;
}

namespace Rocket::Core
{
	class HelperObjectBase
	{
	public:
		HelperObjectBase();
		~HelperObjectBase();

		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Color;
		};

		struct MatrixBufferType
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	public:
		virtual void Initialize(ID3D11Device* device) = 0;
		void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		void Render(ID3D11DeviceContext* deviceContext);
		void SetShader(VertexShader* vertexShader, PixelShader* pixelShader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	protected:
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		ID3D11RasterizerState* _renderState;		// 앞에서 만들어진 ComPtr객체를 raw pointer로 받아온 것.
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		int _indexCount;

		DirectX::XMMATRIX _world;	// Transform Matrix
		DirectX::XMMATRIX _view;
		DirectX::XMMATRIX _proj;
	};
}


