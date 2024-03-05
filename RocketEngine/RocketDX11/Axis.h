#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Axis
	{
	public:
		Axis();
		~Axis();

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
		void Initialize(ID3D11Device* device);
		void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		void Render(ID3D11DeviceContext* deviceContext,
			ID3D11VertexShader* vertexShader,
			ID3D11PixelShader* pixelShader,
			ID3D11Buffer* matrixBuffer,
			ID3D11InputLayout* inputLayout
		);

	private:
		void BuildGeometryBuffers(ID3D11Device* device);

	private:
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		ComPtr<ID3D11RasterizerState> _renderState;

		DirectX::XMMATRIX _world;	// Transform Matrix
		DirectX::XMMATRIX _view;
		DirectX::XMMATRIX _proj;
	};
}
