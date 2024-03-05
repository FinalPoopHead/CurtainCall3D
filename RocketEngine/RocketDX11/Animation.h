#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <unordered_map>


namespace Rocket::Core
{
	// structure containing bone information
	struct Bone
	{
		int id = 0;	// position of the bone in final upload array
		DirectX::XMMATRIX offset = DirectX::XMMatrixIdentity();
	};

	// structure to hold node hierarchy
	struct Node
	{
		std::string name = "";
		Bone bone;
		DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixIdentity();
		Node* parent;
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
	struct NodeAnimation
	{
		std::string nodeName;

		std::vector<float> positionTimestamps = {};
		std::vector<float> rotationTimestamps = {};
		std::vector<float> scaleTimestamps = {};

		std::vector<DirectX::XMFLOAT3> positions = {};
		std::vector<DirectX::XMFLOAT4> rotations = {};
		std::vector<DirectX::XMFLOAT3> scales = {};
	};

	// structure containing animation information
	struct Animation
	{
		float duration = 0.0f;
		float ticksPerSecond = 1.0f;
		float accumulatedTime = 0.0f;
		bool isLoop = true;
		bool isEnd = false;		// flag for not loop animations
		std::vector<NodeAnimation*> nodeAnimations;
	};
}
