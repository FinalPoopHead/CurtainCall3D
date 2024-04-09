#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <string>

#include "IResource.h"
#include "MathHeader.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Texture;
	class VertexShader;
	class PixelShader;
}

namespace Rocket::Core
{
	class CubeMap : public IResource
	{
	public:
		CubeMap();
		~CubeMap();

	public:
		void Initialize(ID3D11Device* device);
		void Render(ID3D11DeviceContext* deviceContext);
		void LoadTexture(const std::string& fileName);
		void SetShader(VertexShader* vertexShader, PixelShader* pixelShader);

	private:
		void BuildGeometryBuffers(ID3D11Device* device);

	private:
		Texture* _texture;
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		ComPtr<ID3D11RasterizerState> _cubeMapRenderState;
		ComPtr<ID3D11SamplerState> _samplerState;
		int _vertexCount;
		int _indexCount;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}
