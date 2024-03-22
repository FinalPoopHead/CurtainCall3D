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
#include "../RocketCommon/IResourceManager.h"
#include "../RocketCommon/GraphicsEnum.h"
#include "ModelStruct.h"
#include "../RocketCommon/RawModelStruct.h"

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
	class CubeMap;
}

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

		/// 앞 단에서 읽은 RawModel을 이용해서 리소스들을 생성하고 저장한다.
		virtual void LoadModel(const std::string& fileName, const RawModel* rawModel) override;

		CubeMesh* GetCubeMesh() const { return _cubeMesh; }
		Mesh* GetMesh(eMeshType meshType) const;
		Mesh* GetMesh(const std::string& fileName);
		Model* GetModel(const std::string& fileName);
		Texture* GetTexture(std::string fileName);
		Texture* GetDefaultTexture() const { return _defaultTexture; }
		Material* GetDefaultMaterial() const { return _defaultMaterial; }
		CubeMap* GetDefaultCubeMap() const { return _cubeMap; }
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
		StaticModel* ProcessStaticModel(const std::string& fileName, const RawModel* rawModel);
		DynamicModel* ProcessDynamicModel(const std::string& fileName, const RawModel* rawModel);
		Node* ProcessRawNodeRecur(const RawNode* rawNode);	// RawNode to Node, Return Root Node
		StaticMesh* ProcessStaticMesh(const RawMesh* rawMesh);
		SkinnedMesh* ProcessSkinnedMesh(const RawMesh* rawMesh);

	private:
		void CreateRenderStates();
		Texture* LoadTextureFile(std::string fileName);

	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;

		// 기본 메쉬들
		CubeMesh* _cubeMesh;
		SphereMesh* _sphereMesh;

		// TODO : 이거 여기있어도 되나? 성격상으로는 ObjectManager에 있어야 할 것 같은데..
		// 기본 큐브맵
		CubeMap* _cubeMap;	

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
		std::unordered_map<std::string, Mesh*> _meshes;
	};
}
