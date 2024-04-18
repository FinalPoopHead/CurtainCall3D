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
/*		void SetIBLCreatingShader(VertexShader* vertexShader, PixelShader* pixelShader);*/

		// 임시
		Texture* GetTexture() const { return _texture; }
		Texture* GetIrradianceTexture() const { return _irradianceTexture; }
		ID3D11ShaderResourceView** GetIrradianceTextureSRV();
		Texture* GetPrefilteredTexture() const { return _prefilteredTexture; }
		ID3D11ShaderResourceView** GetPrefilteredTextureSRV();
		Texture* GetBRDF2DLUTTexture() const { return _BRDF2DLUTTexture; }
		ID3D11ShaderResourceView** GetBRDF2DLUTTextureSRV();
		ID3D11SamplerState** GetSamplerState() { return _samplerState.GetAddressOf(); }

	private:
		void BuildGeometryBuffers(ID3D11Device* device);
/*		void CreateIBLTextures(ID3D11DeviceContext* deviceContext);*/

	private:
		Texture* _texture;
		Texture* _irradianceTexture;
		Texture* _prefilteredTexture;
		Texture* _BRDF2DLUTTexture;
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		ComPtr<ID3D11RasterizerState> _cubeMapRenderState;
		ComPtr<ID3D11SamplerState> _samplerState;
		int _vertexCount;
		int _indexCount;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		// IBL Texture 생성용
// 		ComPtr<ID3D11RenderTargetView> _renderTargetView[6];
// 		ComPtr<ID3D11Texture2D> _IrradianceTexture[6];
// 		ComPtr<ID3D11ShaderResourceView> _IrradianceTextureSRV[6];
// 		VertexShader* _IBLCreatingVS;
// 		PixelShader* _IBLCreatingPS;
	};
}
