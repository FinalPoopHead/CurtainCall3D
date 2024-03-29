#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <unordered_map>
#include <SpriteFont.h>
#include <vector>
#include <GeometricPrimitive.h>
#include <memory>
#include <string>
#include <wrl.h>

#include "Singleton.h"
#include "../RocketCommon/IResourceManager.h"
#include "../RocketCommon/GraphicsEnum.h"
#include "ModelStruct.h"
#include "../RocketCommon/RawModelStruct.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include "Model.h"
#include "mesh.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class ResourceManager : public Singleton<ResourceManager>, public IResourceManager
	{
		friend Singleton;
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
		void Finalize();

		/// 앞 단에서 읽은 RawModel을 이용해서 리소스들을 생성하고 저장한다.
		virtual void LoadModel(const std::string& fileName, const RawModel* rawModel) override;

		CubeMesh* GetCubeMesh() const { return _cubeMesh.get(); }
		Mesh* GetMesh(eMeshType meshType) const;
		Mesh* GetMesh(const std::string& fileName);
		Model* GetModel(const std::string& fileName);
		Texture* GetTexture(std::string fileName);
		Texture* GetDefaultTexture() const { return _defaultTexture; }
		Material* GetDefaultMaterial() const { return _defaultMaterial.get(); }		
		VertexShader* GetVertexShader(const std::string& name);
		PixelShader* GetPixelShader(const std::string& name);
		DirectX::SpriteFont* GetDefaultFont() const;

		DirectX::DX11::GeometricPrimitive* GetCubePrimitive() { return _cubePrimitive.get(); }
		DirectX::DX11::GeometricPrimitive* GetSpherePrimitive() { return _spherePrimitive.get(); }
		DirectX::DX11::GeometricPrimitive* GetCylinderPrimitive() { return _cylinderPrimitive.get(); }

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11RasterizerState* GetRenderState(eRenderState eState);

	private:
		StaticModel* ProcessStaticModel(const std::string& fileName, const RawModel* rawModel);
		DynamicModel* ProcessDynamicModel(const std::string& fileName, const RawModel* rawModel);
		Node* ProcessRawNodeRecur(const RawNode* rawNode);	// RawNode to Node, Return Root Node
		StaticMesh* ProcessStaticMesh(const RawMesh* rawMesh);
		SkinnedMesh* ProcessSkinnedMesh(const RawMesh* rawMesh);

	private:
		void CreateRenderStates();

	private:
		void DeleteNodeRecur(Node* node);

	private:
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		// 기본 메쉬들
		std::unique_ptr<CubeMesh> _cubeMesh;
		std::unique_ptr<SphereMesh> _sphereMesh;

		// 기본 머터리얼
		std::unique_ptr<Material> _defaultMaterial;

		// 기본 텍스쳐
		Texture* _defaultTexture;

		// 기본 폰트 들고있음
		std::unique_ptr<DirectX::SpriteFont> _defaultFont;

		// DXTK Primitive
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _cubePrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _spherePrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _cylinderPrimitive;

		std::unordered_map<std::string, std::unique_ptr<VertexShader>> _vertexShaders;
		std::unordered_map<std::string, std::unique_ptr<PixelShader>> _pixelShaders;
		std::unordered_map<std::string, std::unique_ptr<Texture>> _textures;
		std::vector<ComPtr<ID3D11RasterizerState>> _renderStates;
		std::unordered_map<std::string, std::unique_ptr<Model>> _models;
		std::unordered_map<std::string, std::unique_ptr<Mesh>> _meshes;
	};
}
