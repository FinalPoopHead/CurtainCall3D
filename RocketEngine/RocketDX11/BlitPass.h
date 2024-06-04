#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class VertexShader;
	class PixelShader;
	class DeferredBuffers;
	class Camera;
}

namespace Rocket::Core
{
	class BlitPass
	{
	public:
		BlitPass();
		~BlitPass();

	public:
		void Initialize(ID3D11Device* device, VertexShader* vertexShader, PixelShader* pixelShader);
		void Render(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** srv);

	private:
		void CreateSamplerState(ID3D11Device* device);

	private:
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		ComPtr<ID3D11SamplerState> _sampleState;
	};
}
