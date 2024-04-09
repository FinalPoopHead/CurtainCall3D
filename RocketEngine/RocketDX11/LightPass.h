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
}

namespace Rocket::Core
{
	class LightPass
	{
	public:
		LightPass();
		~LightPass();

	public:
		void Initialize(ID3D11Device* device, VertexShader* vertexShader, PixelShader* pixelShader);
		void Render(ID3D11DeviceContext* deviceContext, DeferredBuffers* g_buffer);

	private:
		void CreateSamplerState(ID3D11Device* device);

	private:
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		ComPtr<ID3D11SamplerState> _sampleState;
	};
}
