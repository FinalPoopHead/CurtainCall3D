#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "MathHeader.h"
#include "IResource.h"

namespace Rocket::Core
{
	class Mesh;
	class StaticMesh;
	class SkinnedMesh;

	struct Model;
	struct StaticModel;
	struct DynamicModel;
	struct Node;
	struct Bone;
	struct NodeAnimationData;
	struct Animation;

	struct Model : public IResource
	{
		// TODO : 여기에 가상 함수가 있는게 맞나?
		virtual std::vector<Mesh*> GetMeshes();

		std::string name;
		Node* rootNode;
		std::unordered_map<std::string, Node*> nodeMap;
	};

	struct StaticModel : public Model
	{
		virtual std::vector<Mesh*> GetMeshes() override;

		std::vector<StaticMesh*> meshes;
	};

	struct DynamicModel : public Model
	{
		virtual std::vector<Mesh*> GetMeshes() override;

		std::vector<SkinnedMesh*> meshes;
		std::unordered_map<std::string, Animation*> animations;		// 애니메이션이 여러개일 수 있으므로
	};

	// structure containing bone information
	struct Bone
	{
		std::string name = "";
		int index = -1;	// position of the bone in final upload array
		Node* bindedNode = nullptr;
		DirectX::XMMATRIX offsetMatrix = DirectX::XMMatrixIdentity();
	};

	// structure to hold node hierarchy
	struct Node
	{
		std::string name = "";
		int index = -1;
		Bone* bindedBone = nullptr;
		DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixIdentity();
		Node* parent = nullptr;
		std::vector<Node*> children = {};

		DirectX::XMMATRIX GetWorldMatrix()
		{
			if (parent)
			{
				return transformMatrix * parent->GetWorldMatrix();
			}
			else
			{
				return transformMatrix;
			}
		}
	};

	// structure containing each node's animation information in one animation
	struct NodeAnimationData
	{
		std::string nodeName;

		std::vector<float> positionTimestamps = {};
		std::vector<float> rotationTimestamps = {};
		std::vector<float> scaleTimestamps = {};

		std::vector<Vector3> positions = {};
		std::vector<Vector4> rotations = {};
		std::vector<Vector3> scales = {};
	};

	// structure containing animation information
	struct Animation
	{
		std::string name = "";
		float duration = 0.0f;
		float ticksPerSecond = 1.0f;
		float accumulatedTime = 0.0f;
		std::vector<NodeAnimationData*> nodeAnimations;
	};
}
