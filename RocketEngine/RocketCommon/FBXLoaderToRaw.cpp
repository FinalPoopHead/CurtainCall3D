#include "FBXLoaderToRaw.h"
#include "AssimpMathConverter.h"

const std::string MODEL_PATH = "Resources/Models/";

namespace Rocket::Core
{
	FBXLoaderToRaw::FBXLoaderToRaw()
	{

	}

	FBXLoaderToRaw::~FBXLoaderToRaw()
	{

	}

	RawModel* FBXLoaderToRaw::LoadFBXFile(std::string fileName)
	{
		std::string fileNameWithExtension;

		/// 경로 제외하기 위한 로직
		size_t slashIndex = fileName.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			fileNameWithExtension = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
		}
		else
		{
			fileNameWithExtension = fileName;
		}

		std::string path = MODEL_PATH + fileNameWithExtension;

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate
			| aiProcess_ConvertToLeftHanded
			| aiProcess_PopulateArmatureData
			| aiProcess_CalcTangentSpace
			| aiProcess_LimitBoneWeights
		);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
			assert(false);
		}

		/// 모델 데이터 로드 (모델,메쉬,노드,본)
		_resultModel = new RawModel();
		_resultModel->name = fileName;

		// 1. 우선 노드를 쭉 읽으면서 Node의 Hierarchy를 만든다.
		_resultModel->rootNode = ReadNodeHierarchy(scene->mRootNode);

		// 2. 노드를 읽어서 메쉬데이터를 처리한다. 이때 앞서 만든 노드 Hierarchy를 이용해 Bone 데이터도 처리한다.
		ProcessNode(scene->mRootNode, scene);
		ForSungchan(_resultModel->rootNode);

		// 3. 애니메이션 정보를 처리한다.
		if (scene->HasAnimations())
		{
			LoadAnimation(scene);			// 애니메이션 데이터 로드
		}

		_aiNodeToNodeMap.clear();

		return _resultModel;
	}

	void FBXLoaderToRaw::ProcessNode(aiNode* ainode, const aiScene* scene)
	{
		for (UINT i = 0; i < ainode->mNumMeshes; ++i)
		{
			RawMesh* mesh = ProcessMesh(scene->mMeshes[ainode->mMeshes[i]], scene);

			RawNode* node = _aiNodeToNodeMap.at(ainode);
			mesh->BindNode(node);
			node->meshes.emplace_back(mesh);
			_resultModel->meshes.emplace_back(mesh);
		}

		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			ProcessNode(ainode->mChildren[i], scene);
		}
	}

	RawMesh* FBXLoaderToRaw::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<RawVertex> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			RawVertex vertex;

			// process position
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// process uv
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			// process normal
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// process tangent
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			// 우선 0으로 초기화
			vertex.nodeIndex = 0;
			vertex.boneIndices = DirectX::XMUINT4{ 0, 0, 0, 0 };
			vertex.weights = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

		// Load indices
		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace& face = mesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 본데이터 처리
		std::vector<uint32_t> boneIndecesPerVertex;
		boneIndecesPerVertex.resize(vertices.size());

		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* aibone = mesh->mBones[i];

			RawBone* bone = new RawBone();
			bone->name = aibone->mName.C_Str();
			bone->index = _aiNodeToNodeMap.at(aibone->mNode)->index;
			int test = bone->index;
			bone->bindedNode = _aiNodeToNodeMap.at(aibone->mNode);
			bone->offsetMatrix = AIMatrix4x4ToXMMatrix(aibone->mOffsetMatrix.Transpose());

 			_aiNodeToNodeMap.at(aibone->mNode)->bindedBone = bone;

			for (unsigned int j = 0; j < aibone->mNumWeights; j++)
			{
				int vertexIndex = aibone->mWeights[j].mVertexId;
				float weight = aibone->mWeights[j].mWeight;

				uint32_t boneCount = boneIndecesPerVertex[vertexIndex];

				switch (boneCount)
				{
				case 0:
					vertices[vertexIndex].weights.x = weight;
					vertices[vertexIndex].boneIndices.x = bone->index;
					break;
				case 1:
					vertices[vertexIndex].weights.y = weight;
					vertices[vertexIndex].boneIndices.y = bone->index;
					break;
				case 2:
					vertices[vertexIndex].weights.z = weight;
					vertices[vertexIndex].boneIndices.z = bone->index;
					break;
				case 3:
					vertices[vertexIndex].weights.w = weight;
					vertices[vertexIndex].boneIndices.w = bone->index;
					break;
				default:
					break;
				}
				boneIndecesPerVertex[vertexIndex]++;
				//vertices[vertexIndex].nodeIndex = bone->index;	// 버텍스가 여러 본(노드)에 영향을 받으므로 셰이더에서 그것을 이용해 연산한다.
			}
		}

		// 머터리얼 처리
		RawMaterial* resultMaterial = nullptr;

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				if (material->GetTextureCount((aiTextureType)i) > 0)
				{
					resultMaterial = LoadMaterialTextures(material, (aiTextureType)i, scene);
				}
			}
		}

		RawMesh* resultMesh = new RawMesh();
		resultMesh->name = mesh->mName.C_Str();
		resultMesh->vertices = vertices;
		resultMesh->indices = indices;
		resultMesh->material = resultMaterial;

		return resultMesh;
	}

	RawMaterial* FBXLoaderToRaw::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
	{
		RawMaterial* resultMaterial = new RawMaterial();

		UINT textureCount = material->GetTextureCount(type);

		for (UINT i = 0; i < textureCount; ++i)
		{
			aiString path;
			material->GetTexture(type, i, &path);
			std::string s = std::string(path.C_Str());
			std::string fileName = s.substr(s.find_last_of("/\\") + 1, s.length() - s.find_last_of("/\\"));

			switch (type)
			{
					// Legacy
				case aiTextureType_DIFFUSE:
					resultMaterial->diffuseTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->diffuseTexture);
					break;
				case aiTextureType_SPECULAR:
					resultMaterial->specularTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->specularTexture);
					break;
				case aiTextureType_AMBIENT:
					resultMaterial->ambientTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->ambientTexture);
					break;
				case aiTextureType_EMISSIVE:
					resultMaterial->emissiveTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->emissiveTexture);
					break;
				case aiTextureType_HEIGHT:
					resultMaterial->heightTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->heightTexture);
					break;
				case aiTextureType_NORMALS:
					resultMaterial->normalTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->normalTexture);
					break;
				case aiTextureType_SHININESS:
					resultMaterial->shininessTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->shininessTexture);
					break;
				case aiTextureType_OPACITY:
					resultMaterial->opacityTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->opacityTexture);
					break;
				case aiTextureType_DISPLACEMENT:
					resultMaterial->displacementTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->displacementTexture);
					break;
				case aiTextureType_LIGHTMAP:
					resultMaterial->lightmapTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->lightmapTexture);
					break;
				case aiTextureType_REFLECTION:
					resultMaterial->reflectionTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->reflectionTexture);
					break;

					// PBR
				case aiTextureType_BASE_COLOR:
					resultMaterial->baseColorTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->baseColorTexture);
					break;
				case aiTextureType_NORMAL_CAMERA:
					resultMaterial->normalCameraTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->normalCameraTexture);
					break;
				case aiTextureType_EMISSION_COLOR:
					resultMaterial->emissiveColorTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->emissiveColorTexture);
					break;
				case aiTextureType_METALNESS:
					resultMaterial->metallicTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->metallicTexture);
					break;
				case aiTextureType_DIFFUSE_ROUGHNESS:
					resultMaterial->roughnessTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->roughnessTexture);
					break;
				case aiTextureType_AMBIENT_OCCLUSION:
					resultMaterial->ambientOcclusionTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->ambientOcclusionTexture);
					break;
				case aiTextureType_SHEEN:
					resultMaterial->sheenTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->sheenTexture);
					break;
				case aiTextureType_TRANSMISSION:
					resultMaterial->transmissionTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->transmissionTexture);
					break;
				case aiTextureType_UNKNOWN:
					resultMaterial->unknownTexture = new RawTexture({ fileName, fileName });
					resultMaterial->textures.push_back(resultMaterial->unknownTexture);
					break;
			}
		}

		return resultMaterial;
	}

	void FBXLoaderToRaw::LoadAnimation(const aiScene* scene)
	{
		if (!scene->HasAnimations())
		{
			return;
		}

		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
		// numChannels == numBones
		UINT animCount = scene->mNumAnimations;
		for (UINT i = 0; i < animCount; ++i)
		{
			auto aiAnim = scene->mAnimations[i];
			RawAnimation* myAnimStruct = new RawAnimation();

			myAnimStruct->name = aiAnim->mName.C_Str();
			myAnimStruct->duration = aiAnim->mDuration;

			// mTicksPerSeciond 이거 그건가? 초당 프레임 수?
			if (aiAnim->mTicksPerSecond != 0.0)
			{
				myAnimStruct->ticksPerSecond = aiAnim->mTicksPerSecond;
			}
			else
			{
				myAnimStruct->ticksPerSecond = 30.0f;
			}

			for (UINT j = 0; j < aiAnim->mNumChannels; ++j)
			{
				const aiNodeAnim* aiNodeAnim = aiAnim->mChannels[j];
				RawNodeAnimationData* myNodeAnim = new RawNodeAnimationData();

				myNodeAnim->nodeName = aiNodeAnim->mNodeName.C_Str();

				for (unsigned int k = 0; k < aiNodeAnim->mNumPositionKeys; ++k)
				{
					myNodeAnim->positionTimestamps.push_back(aiNodeAnim->mPositionKeys[k].mTime);
					myNodeAnim->positions.push_back(AIVec3ToXMFloat3(aiNodeAnim->mPositionKeys[k].mValue));
				}
				for (unsigned int k = 0; k < aiNodeAnim->mNumRotationKeys; ++k)
				{
					myNodeAnim->rotationTimestamps.push_back(aiNodeAnim->mRotationKeys[k].mTime);
					myNodeAnim->rotations.push_back(AIQuaternionToXMFloat4(aiNodeAnim->mRotationKeys[k].mValue));
				}
				for (unsigned int k = 0; k < aiNodeAnim->mNumScalingKeys; ++k)
				{
					myNodeAnim->scaleTimestamps.push_back(aiNodeAnim->mScalingKeys[k].mTime);
					myNodeAnim->scales.push_back(AIVec3ToXMFloat3(aiNodeAnim->mScalingKeys[k].mValue));
				}

				myAnimStruct->nodeAnimations.push_back(myNodeAnim);
			}

			_resultModel->animations.insert({ myAnimStruct->name, myAnimStruct });
		}
	}

	RawNode* FBXLoaderToRaw::ReadNodeHierarchy(aiNode* ainode)
	{
		UINT index = 0;
		RawNode* rootNode = new RawNode();

		ReadNodeRecur(rootNode, ainode, index);

		return rootNode;
	}

	void FBXLoaderToRaw::ReadNodeRecur(RawNode* node, aiNode* ainode, UINT& index)
	{
		node->name = ainode->mName.C_Str();
		// Assimp가 Column Major로 Matrix를 읽어오므로 Row Major 하게 Transpose 해준다.
		node->transformMatrix = AIMatrix4x4ToXMMatrix(ainode->mTransformation.Transpose());
		node->index = index;
		index++;

		_aiNodeToNodeMap.insert({ ainode,node });

		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			RawNode* newNode = new RawNode();
			node->children.emplace_back(newNode);
			newNode->parent = node;

			ReadNodeRecur(newNode, ainode->mChildren[i], index);
		}
	}

	void FBXLoaderToRaw::ForSungchan(RawNode* node)
	{
		if (node->bindedBone)
		{
			_sungchanBoneCount++;
		}

		for (int i = 0; i < node->children.size(); i++)
		{
			ForSungchan(node->children[i]);
		}
	}

}
