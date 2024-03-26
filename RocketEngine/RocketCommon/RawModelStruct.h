#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "MathHeader.h"

/// 그래픽스에 있는 ModelStruct를 복사해와서 구성할 예정.
/// Loader들을 통해 여기에 있는 구조체를 생성하고 데이터를 채워넣을 예정.
/// Vertex에 대한 명세도 여기로 가져와야 되려나?

namespace Rocket::Core
{
	struct RawModel;
	struct RawNode;
	struct RawBone;
	struct RawMesh;
	struct RawVertex;
	struct RawMaterial;
	struct RawTexture;
		   
	struct RawNodeAnimationData;
	struct RawAnimation;

	struct RawModel
	{
		std::string name = "";
		RawNode* rootNode = nullptr;
		std::vector<RawMesh*> meshes;
		std::unordered_map<std::string, RawAnimation*> animations;
	};

	// structure to hold node hierarchy
	struct RawNode
	{
		std::string name = "";
		int index = -1;
		RawBone* bindedBone = nullptr;
		Matrix transformMatrix = Matrix::Identity;
		RawNode* parent = nullptr;
		std::vector<RawNode*> children = {};
		std::vector<RawMesh*> meshes = {};
	};

	// structure containing bone information
	struct RawBone
	{
		std::string name = "";
		int index = -1;	// position of the bone in final upload array
		RawNode* bindedNode = nullptr;
		Matrix offsetMatrix = Matrix::Identity;
	};

	struct RawVertex
	{
		Vector3 position = Vector3::Zero;
		Vector2 UV = Vector2::Zero;
		Vector3 normal = Vector3::Zero;
		Vector3 tangent = Vector3::Zero;
		UINT nodeIndex = 0;
		Vector4 weights = Vector4::Zero;			// 최대 4개의 본에 대한 가중치
		DirectX::XMUINT4 boneIndices = {};		// 최대 4개의 본에 대한 인덱스
	};

	struct RawMesh
	{
		void BindNode(RawNode* node)
		{
			bindedNode = node;

			for (auto& vertex : vertices)
			{
				vertex.nodeIndex = node->index;
			}
		}

		std::string name = "";
		RawNode* bindedNode = nullptr;
		RawMaterial* material = nullptr;
		std::vector<RawVertex> vertices = {};
		std::vector<unsigned int> indices = {};
	};

	struct RawMaterial
	{
		std::vector<RawTexture*> textures = {};

		// Legacy
		RawTexture* diffuseTexture = nullptr;
		RawTexture* specularTexture = nullptr;
		RawTexture* ambientTexture = nullptr;
		RawTexture* emissiveTexture = nullptr;
		RawTexture* heightTexture = nullptr;
		RawTexture* normalTexture = nullptr;
		RawTexture* shininessTexture = nullptr;
		RawTexture* opacityTexture = nullptr;
		RawTexture* displacementTexture = nullptr;
		RawTexture* lightmapTexture = nullptr;
		RawTexture* reflectionTexture = nullptr;

		// PBR
		RawTexture* baseColorTexture = nullptr;
		RawTexture* normalCameraTexture = nullptr;
		RawTexture* emissiveColorTexture = nullptr;
		RawTexture* metallicTexture = nullptr;
		RawTexture* roughnessTexture = nullptr;
		RawTexture* ambientOcclusionTexture = nullptr;
		RawTexture* sheenTexture = nullptr;
		RawTexture* clearcoatTexture = nullptr;
		RawTexture* transmissionTexture = nullptr;
		RawTexture* unknownTexture = nullptr;
	};

	struct RawTexture
	{
		std::string name = "";
		std::string path = "";
	};

	// structure containing each node's animation information in one animation
	struct RawNodeAnimationData
	{
		std::string nodeName = "";

		std::vector<double> positionTimestamps = {};
		std::vector<double> rotationTimestamps = {};
		std::vector<double> scaleTimestamps = {};

		std::vector<Vector3> positions = {};
		std::vector<Vector4> rotations = {};
		std::vector<Vector3> scales = {};
	};

	// structure containing animation information
	struct RawAnimation
	{
		std::string name = "";
		double duration = 0.0;
		double ticksPerSecond = 1.0;
		float accumulatedTime = 0.0f;
		std::vector<RawNodeAnimationData*> nodeAnimations;
	};
}
