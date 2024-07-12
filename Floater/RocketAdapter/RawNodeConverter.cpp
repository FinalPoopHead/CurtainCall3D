#include "RawNodeConverter.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/ConvString.h"
#include "../FloaterMath/include/Matrix4f.h"

#include "../FloaterRendererCommon/include/RawNode.h"
#include "../FloaterRendererCommon/include/RawSkeleton.h"

#include "RocketObject.h"

#pragma warning(push)
#pragma warning(disable: 26495)
#include "./RocketCommon/RawModelStruct.h"
#include "./RocketCommon/RocketTransform.h"
#pragma warning(pop)

#include <DirectXMath.h>

#pragma comment(lib, "DirectXTK.lib")

#ifdef _DEBUG

#else
#endif // DEBUG



DirectX::XMMATRIX ConvertXMMatrix(const flt::Matrix4f& matrix)
{
	DirectX::XMMATRIX result
	{
		matrix.e[0][0], matrix.e[0][1], matrix.e[0][2], matrix.e[0][3],
		matrix.e[1][0], matrix.e[1][1], matrix.e[1][2], matrix.e[1][3],
		matrix.e[2][0], matrix.e[2][1], matrix.e[2][2], matrix.e[2][3],
		matrix.e[3][0], matrix.e[3][1], matrix.e[3][2], matrix.e[3][3]
	};

	return result;
}


using RkModel = Rocket::Core::RawModel;
using RkNode = Rocket::Core::RawNode;
using RkBone = Rocket::Core::RawBone;
using RkMesh = Rocket::Core::RawMesh;
using RkMaterial = Rocket::Core::RawMaterial;
using RkRawAnimation = Rocket::Core::RawAnimation;


Rocket::Core::RawModel* flt::ConvertModel(const flt::RawNode& rootNode)
{
	RkModel* model = new(std::nothrow) Rocket::Core::RawModel();
	ASSERT(model, "Failed to allocate memory for model");

	// TODO : 파일명을 넣어줘야함.
	model->name = ToString(rootNode.name);
	model->rootNode = ConvertfltRawNodeTorocketRawNodeRecursive(rootNode);
	CopyMeshesToModel(model->rootNode, model);

	// TODO : 본 애니메이션 말고도 Node애니메이션도 넣어야하지만 나중에.
	if (rootNode.skeleton)
	{
		for (const auto& animation : rootNode.skeleton->animations)
		{
			RkRawAnimation* rocketAnimation = new(std::nothrow) Rocket::Core::RawAnimation();
			ASSERT(rocketAnimation, "Failed to allocate memory for animation");

			rocketAnimation->nodeAnimations.reserve(animation.clips.size());
			for (int i = 0; i < animation.clips.size(); ++i)
			{
				Rocket::Core::RawNodeAnimationData* rocketAnimData = new(std::nothrow) Rocket::Core::RawNodeAnimationData();
				ASSERT(rocketAnimData, "Failed to allocate memory for channel");
				rocketAnimData->nodeName = ToString(rootNode.skeleton->bones[i].name);

				for (int j = 0; j < animation.clips[i].keyPosition.size(); ++j)
				{
					rocketAnimData->positions.push_back({ animation.clips[i].keyPosition[j].position.x, animation.clips[i].keyPosition[j].position.y, animation.clips[i].keyPosition[j].position.z });
					rocketAnimData->positionTimestamps.push_back(animation.clips[i].keyPosition[j].time);
				}
				if(animation.clips[i].keyPosition.size() == 0)
				{
					Vector4f pos = rootNode.skeleton->bones[i].tr.GetLocalPosition();
					rocketAnimData->positions.push_back({ pos.x, pos.y, pos.z });
					rocketAnimData->positionTimestamps.push_back(0);
				}

				for (int j = 0; j < animation.clips[i].keyRotation.size(); ++j)
				{
					rocketAnimData->rotations.push_back({ animation.clips[i].keyRotation[j].rotation.x, animation.clips[i].keyRotation[j].rotation.y, animation.clips[i].keyRotation[j].rotation.z, animation.clips[i].keyRotation[j].rotation.w });
					rocketAnimData->rotationTimestamps.push_back(animation.clips[i].keyRotation[j].time);
				}
				if (animation.clips[i].keyRotation.size() == 0)
				{
					Quaternion rot = rootNode.skeleton->bones[i].tr.GetLocalRotation();
					rocketAnimData->rotations.push_back({ rot.x, rot.y, rot.z, rot.w });
					rocketAnimData->rotationTimestamps.push_back(0);
				}

				for (int j = 0; j < animation.clips[i].keyScale.size(); ++j)
				{
					rocketAnimData->scales.push_back({ animation.clips[i].keyScale[j].scale.x, animation.clips[i].keyScale[j].scale.y, animation.clips[i].keyScale[j].scale.z });
					rocketAnimData->scaleTimestamps.push_back(animation.clips[i].keyScale[j].time);
				}
				if (animation.clips[i].keyScale.size() == 0)
				{
					Vector4f scale = rootNode.skeleton->bones[i].tr.GetLocalScale();
					rocketAnimData->scales.push_back({ scale.x, scale.y, scale.z });
					rocketAnimData->scaleTimestamps.push_back(0);
				}

				rocketAnimation->nodeAnimations.push_back(rocketAnimData);
			}

			rocketAnimation->name = ToString(animation.name);
			rocketAnimation->duration = animation.duration;
			rocketAnimation->ticksPerSecond = animation.ticksPerSecond;

			model->animations[rocketAnimation->name] = rocketAnimation;
		}
	}

	return model;
}

Rocket::Core::RawNode* flt::ConvertfltRawNodeTorocketRawNodeRecursive(const flt::RawNode& node)
{
	RkNode* rocketNode = new(std::nothrow) Rocket::Core::RawNode();
	ASSERT(rocketNode, "Failed to allocate memory for node");

	rocketNode->name = ToString(node.name);
	// TODO : index를 넣어줘야함.
	rocketNode->index = -1;
	rocketNode->bindedBone = nullptr;
	rocketNode->transformMatrix = ConvertXMMatrix(node.transform.GetLocalMatrix4f());

	for (const auto& mesh : node.meshes)
	{
		RkMesh* rocketMesh = new(std::nothrow) Rocket::Core::RawMesh();
		ASSERT(rocketMesh, "Failed to allocate memory for mesh");
		rocketMesh->name = ToString(node.name);
		rocketMesh->BindNode(rocketNode);

		rocketMesh->material = new(std::nothrow) RkMaterial();
		ASSERT(rocketMesh->material, "Failed to allocate memory for material");

		// material 정보 입력
		const flt::RawMaterial& material = mesh.material;
		RkMaterial* rocketMaterial = rocketMesh->material;
		for (int i = 0; i < material.MAX_TEXTURES; ++i)
		{
			if (material.textures[i] == nullptr)
			{
				continue;
			}

			// PBR은 일단 할당하지 않음.
			switch (i)
			{
				case flt::RawMaterial::TextureType::ALBEDO_OPACITY:
				{
					rocketMaterial->diffuseTexture = new(std::nothrow) Rocket::Core::RawTexture();
					ASSERT(rocketMaterial->diffuseTexture, "Failed to allocate memory for texture");
					rocketMaterial->diffuseTexture->name = ToString(material.textures[i]->name);
					rocketMaterial->diffuseTexture->path = ToString(material.textures[i]->path);
					rocketMaterial->textures.push_back(rocketMaterial->diffuseTexture);
				}
				break;
				case flt::RawMaterial::TextureType::NORMAL:
				{
					rocketMaterial->normalTexture = new(std::nothrow) Rocket::Core::RawTexture();
					ASSERT(rocketMaterial->normalTexture, "Failed to allocate memory for texture");
					rocketMaterial->normalTexture->name = ToString(material.textures[i]->name);
					rocketMaterial->normalTexture->path = ToString(material.textures[i]->path);
					rocketMaterial->textures.push_back(rocketMaterial->normalTexture);
				}
				break;
				case flt::RawMaterial::TextureType::METALLIC:
				{

				}
				break;
				case flt::RawMaterial::TextureType::ROUGHNESS:
				{

				}
				break;
				case flt::RawMaterial::TextureType::AO:
				{

				}
				break;
				case flt::RawMaterial::TextureType::EMISSIVE:
				{
					rocketMaterial->emissiveTexture = new(std::nothrow) Rocket::Core::RawTexture();
					ASSERT(rocketMaterial->emissiveTexture, "Failed to allocate memory for texture");
					rocketMaterial->emissiveTexture->name = ToString(material.textures[i]->name);
					rocketMaterial->emissiveTexture->path = ToString(material.textures[i]->path);
					rocketMaterial->textures.push_back(rocketMaterial->emissiveTexture);
				}
				break;
				case flt::RawMaterial::TextureType::HEIGHT:
				{

				}
				break;
				case flt::RawMaterial::TextureType::OPACITY:
				{
					rocketMaterial->opacityTexture = new(std::nothrow) Rocket::Core::RawTexture();
					ASSERT(rocketMaterial->opacityTexture, "Failed to allocate memory for texture");
					rocketMaterial->opacityTexture->name = ToString(material.textures[i]->name);
					rocketMaterial->opacityTexture->path = ToString(material.textures[i]->path);
					rocketMaterial->textures.push_back(rocketMaterial->opacityTexture);
				}
				break;
				case flt::RawMaterial::TextureType::UNKNOWN:
				{

				}
				break;

				default:
					//ASSERT(false, "not defined type");
					break;
			}
		}

		// vertex 정보 입력
		rocketMesh->vertices.reserve(mesh.vertices.size());
		for (const flt::RawVertex& vertex : mesh.vertices)
		{
			Rocket::Core::RawVertex rocketVertex;
			rocketVertex.position = DirectX::SimpleMath::Vector3{ vertex.pos.x, vertex.pos.y, vertex.pos.z };
			rocketVertex.UV = DirectX::SimpleMath::Vector2{ vertex.uvs[0].x, vertex.uvs[0].y };
			rocketVertex.normal = DirectX::SimpleMath::Vector3{ vertex.normal.x, vertex.normal.y, vertex.normal.z };
			rocketVertex.tangent = DirectX::SimpleMath::Vector3{ vertex.tangent.x, vertex.tangent.y, vertex.tangent.z };
			rocketVertex.biTangent = DirectX::SimpleMath::Vector3{ vertex.binormal.x, vertex.binormal.y, vertex.binormal.z };
			rocketVertex.nodeIndex = -1;
			for (int i = 0; i < vertex.boneWeights.size(); ++i)
			{
				if (i > 3)
				{
					break;
				}

				switch (i)
				{
					case 0:
					{
						rocketVertex.boneIndices.x = vertex.boneIndices[i];
						rocketVertex.weights.x = vertex.boneWeights[i];
					}
					break;
					case 1:
					{
						rocketVertex.boneIndices.y = vertex.boneIndices[i];
						rocketVertex.weights.y = vertex.boneWeights[i];
					}
					break;
					case 2:
					{
						rocketVertex.boneIndices.z = vertex.boneIndices[i];
						rocketVertex.weights.z = vertex.boneWeights[i];
					}
					break;
					case 3:
					{
						rocketVertex.boneIndices.w = vertex.boneIndices[i];
						rocketVertex.weights.w = vertex.boneWeights[i];
					}
					break;
					default:
						break;
				}
			}
			rocketMesh->vertices.push_back(rocketVertex);
		}

		rocketMesh->indices.reserve(mesh.indices.size());
		for (const auto& index : mesh.indices)
		{
			rocketMesh->indices.push_back((unsigned int)index);
		}

		rocketNode->meshes.push_back(rocketMesh);
	}

	for (const auto& child : node.children)
	{
		RkNode* rocketChild = ConvertfltRawNodeTorocketRawNodeRecursive(*child);
		rocketNode->children.push_back(rocketChild);
		rocketChild->parent = rocketNode;
	}

	if (node.skeleton)
	{
		RkNode* rocketChild = ConvertfltBoneTorocketNodeRecursive(node.skeleton->bones[node.skeleton->rootBoneIndex], &node.skeleton->bones[0], node.skeleton);
		rocketNode->children.push_back(rocketChild);
		rocketChild->parent = rocketNode;
	}

	return rocketNode;
}

Rocket::Core::RawNode* flt::ConvertfltBoneTorocketNodeRecursive(const flt::RawSkeleton::Bone& bone, const flt::RawSkeleton::Bone* const pIndexOffsetBone, const flt::RawSkeleton* const skeleton)
{
	RkNode* rocketNode = new(std::nothrow) Rocket::Core::RawNode();
	ASSERT(rocketNode, "Failed to allocate memory for node");

	rocketNode->name = ToString(bone.name);
	// TODO : index를 넣어줘야함.
	rocketNode->index = (int)(&bone - pIndexOffsetBone);
	rocketNode->bindedBone = new(std::nothrow) RkBone();
	ASSERT(rocketNode->bindedBone, "Failed to allocate memory for bone");
	rocketNode->bindedBone->index = rocketNode->index;
	rocketNode->bindedBone->offsetMatrix = ConvertXMMatrix(skeleton->boneOffsets[rocketNode->index]);
	rocketNode->transformMatrix = ConvertXMMatrix(bone.transform.GetLocalMatrix4f());


	const Transform& transform = bone.transform;
	for (const auto& childTr : transform.GetChildren())
	{
		auto owner = childTr->GetOwner();
		const RawSkeleton::Bone* childBone = (RawSkeleton::Bone*)owner;
		RkNode* rocketChild = ConvertfltBoneTorocketNodeRecursive(*childBone, pIndexOffsetBone, skeleton);
		rocketNode->children.push_back(rocketChild);
		rocketChild->parent = rocketNode;
	}

	return rocketNode;
}

void flt::CopyMeshesToModel(Rocket::Core::RawNode* node, Rocket::Core::RawModel* rocketModel)
{
	for (auto& mesh : node->meshes)
	{
		rocketModel->meshes.push_back(mesh);
	}

	for (auto& child : node->children)
	{
		CopyMeshesToModel(child, rocketModel);
	}
}

void flt::GenerateTransformHierarchyRecursive(Rocket::Core::RocketTransform* rootRkTransform, Transform* rootTransform)
{
	Vector4f pos = rootTransform->GetLocalPosition();
	rootRkTransform->SetLocalPosition({ pos.x, pos.y, pos.z });

	Quaternion rot = rootTransform->GetLocalRotation();
	rootRkTransform->SetLocalRotation({ rot.x, rot.y, rot.z, rot.w });

	Vector4f scale = rootTransform->GetLocalScale();
	rootRkTransform->SetLocalScale({ scale.x, scale.y, scale.z });

	for (auto& child : rootTransform->GetChildren())
	{
		Rocket::Core::RocketTransform* rocketTrChild = new Rocket::Core::RocketTransform();
		rocketTrChild->SetParent(rootRkTransform, false);
		GenerateTransformHierarchyRecursive(rocketTrChild, child);
	}
}

//Rocket::Core::RawNode* flt::ConvertfltRawNodeTorocketRawNode(const RawNode& node)
//{
//	if (node.skeleton)
//	{
//		int i = 0;
//	}
//	else
//	{
//		return ConvertfltRawNodeTorocketRawNodeRecursive(node);
//	}
//}
