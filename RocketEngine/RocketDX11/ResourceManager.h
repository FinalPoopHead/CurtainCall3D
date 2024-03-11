#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <unordered_map>
#include <SpriteFont.h>
#include <vector>
#include <GeometricPrimitive.h>
#include <memory>
#include <string>

#include "Singleton.h"
#include "../GraphicsInterface/GraphicsEnum.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Mesh;
	class CubeMesh;
	class SphereMesh;
	class VertexShader;
	class PixelShader;
	class SpriteRenderer;
	class Texture;
	class Material;
	class FBXLoader;
	struct Model;

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
		friend FBXLoader;
	private:
		ResourceManager();

	public:
		enum class eRenderState
		{
			SOLID,
			WIREFRAME
		};

	public:
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		CubeMesh* GetCubeMesh() const { return _cubeMesh; }
		Mesh* GetMesh(eMeshType meshType) const;
		// std::vector<Mesh*>& GetMeshes(const std::string& fileName);		지금은 Model 베이스로 그리게끔 해놨음.
		Model* GetModel(const std::string& fileName);
		Texture* GetTexture(std::string fileName);
		Texture* GetDefaultTexture() const { return _defaultTexture; }
		Material* GetDefaultMaterial() const { return _defaultMaterial; }
		VertexShader* GetVertexShader(const std::string& name);
		PixelShader* GetPixelShader(const std::string& name);
		DirectX::SpriteFont* GetDefaultFont();

		DirectX::DX11::GeometricPrimitive* GetCubePrimitive() { return _cubePrimitive.get(); }
		DirectX::DX11::GeometricPrimitive* GetSpherePrimitive() { return _spherePrimitive.get(); }
		DirectX::DX11::GeometricPrimitive* GetCylinderPrimitive() { return _cylinderPrimitive.get(); }

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11RasterizerState* GetRenderState(eRenderState eState);

	private:
		void CreateRenderStates();
		Texture* LoadTextureFile(std::string fileName);

	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;
		FBXLoader* _fbxLoader;

		// 기본 메쉬들
		CubeMesh* _cubeMesh;
		SphereMesh* _sphereMesh;

		// 기본 머터리얼
		Material* _defaultMaterial;

		// 기본 텍스쳐
		Texture* _defaultTexture;

		// 기본 폰트 들고있음
		DirectX::SpriteFont* _defaultFont;

		// DXTK Primitive
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _cubePrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _spherePrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _cylinderPrimitive;

		std::unordered_map<std::string, VertexShader*> _vertexShaders;
		std::unordered_map<std::string, PixelShader*> _pixelShaders;
		std::unordered_map<std::string, Texture*> _textures;
		std::vector<ID3D11RasterizerState*> _renderStates;
		std::unordered_map<std::string, Model*> _models;
	};
}
