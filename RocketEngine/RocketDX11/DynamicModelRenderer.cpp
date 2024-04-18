#include "DynamicModelRenderer.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"
#include "ResourceManager.h"
#include "SkinnedMesh.h"
#include "../RocketCommon/RocketTransform.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "DirectionalLight.h"

namespace Rocket::Core
{
	DynamicModelRenderer::DynamicModelRenderer()
		: _model(nullptr)
		, _material(nullptr)
		, _isActive(true)
		, _worldTM(Matrix::Identity)
		, _animatedRootNode(nullptr)
		, _animationTime(0.0)
		, _animationTick(0.0)
		, _rootTransform(nullptr)
		, _boundingBox()
	{

	}

	DynamicModelRenderer::~DynamicModelRenderer()
	{
		// TODO : 원래는 ResourceManager에서 unique_ptr로 관리하고 있어야 하는데, 지금은 ObjectManager에서 생성할때 new Material()해서 받아온 것임.
		//			ResourceManager에서 받아오도록 수정해야함.
		delete _material;
		DeleteCopiedNodeRecur(_animatedRootNode);
	}

	void DynamicModelRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}

	void DynamicModelRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void DynamicModelRenderer::LoadModel(const std::string& fileName)
	{
		_model = reinterpret_cast<DynamicModel*>(ResourceManager::Instance().GetModel(fileName));
		if (_model == nullptr)
		{
			MessageBox(NULL, TEXT("모델이 없습니다."), TEXT("모델 로드 실패"), MB_OK);
			return;
		}
		_animatedRootNode = CopyNodeData(_model->rootNode);
		FindArmatureRootRecur(&_armatureRootNode, _animatedRootNode);

		// BoundingBox 생성
		std::vector<DirectX::XMFLOAT3> points;

		for (auto& mesh : _model->meshes)
		{
			for (auto& vertex : mesh->GetVertices())
			{
				points.push_back(vertex.position);
			}
		}

		DirectX::BoundingBox temp;
		DirectX::BoundingBox::CreateFromPoints(temp, points.size(), points.data(), sizeof(DirectX::XMFLOAT3));

		//_boundingBox = temp;
		DirectX::BoundingOrientedBox::CreateFromBoundingBox(_boundingBox, temp);
	}

	void DynamicModelRenderer::LoadBaseColorTexture(std::string fileName)
	{
		_material->SetBaseColorTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::BindTransform(RocketTransform* rootTransform)
	{
		BindTransformRecur(rootTransform, _animatedRootNode);
	}

	void DynamicModelRenderer::UpdateAnimation(float deltaTime, bool isCulled /*= false*/)
	{
		if (_model->animations.empty())
		{
			return;
		}

		// TODO : 외부에서 애니메이션 세팅하고 재생할 수 있게 바꿔야함. 지금은 임시로 첫번째 애니메이션만 실행함.
		Animation* anim = _model->animations.begin()->second;
		//anim = _model->animations.at("Ani_Monster2_BattleIdle");
		_nowAnimationName = anim->name;

		//if (_animationTime == anim->duration)
		//{
		//	return;
		//}

		_animationTime += deltaTime;
		_animationTick = _animationTime * anim->ticksPerSecond;

		if (_animationTick > anim->duration)
		{
			if (_isLoop)
			{
				double secondPerTick = anim->duration / anim->ticksPerSecond;;
				int count = 0;
				while (secondPerTick * (count + 1) < _animationTime)
				{
					count++;
				}
				_animationTime -= count * secondPerTick;
				_animationTick = _animationTime * anim->ticksPerSecond;
			}
			else
			{
				_animationTime = anim->duration / anim->ticksPerSecond;
				_animationTick = _animationTime * anim->ticksPerSecond;
			}
		}

		if (isCulled)
		{
			return;
		}

		for (auto& nodeAnim : anim->nodeAnimations)
		{
			Vector3 position;
			Vector4 rotation;
			Vector3 scale;

			Node* node = _animatedNodeMap[nodeAnim->nodeName];

			// TODO : nodeAnim이 없을 경우도 있다? 그때를 대비한 코드를 작성해라. 내꺼에서는 이 범위기반 for문에서 없으면 애초에 들어오지 않으나. 들어왔지만 data가 없는 경우를 주의해야 한다.

			// Position
			{
				int positionIndex = 0;
				for (int i = 0; i < nodeAnim->positionTimestamps.size(); i++)
				{
					if (_animationTick < nodeAnim->positionTimestamps[i])
					{
						positionIndex = i;		// i-1 < _animationTickTime < i
						break;
					}
				}

				if (positionIndex == 0)
				{
					position = nodeAnim->positions[0];
				}
				else
				{
					double t = (_animationTick - nodeAnim->positionTimestamps[positionIndex - 1]) / (nodeAnim->positionTimestamps[positionIndex] - nodeAnim->positionTimestamps[positionIndex - 1]);
					position = DirectX::XMVectorLerp(nodeAnim->positions[positionIndex - 1], nodeAnim->positions[positionIndex], (float)t);
				}
			}

			// Rotation
			{
				int rotationIndex = 0;
				for (int i = 0; i < nodeAnim->rotationTimestamps.size(); i++)
				{
					if (_animationTick < nodeAnim->rotationTimestamps[i])
					{
						rotationIndex = i;
						break;
					}
				}

				if (rotationIndex == 0)
				{
					rotation = nodeAnim->rotations[0];
				}
				else
				{
					double t = (_animationTick - nodeAnim->rotationTimestamps[rotationIndex - 1]) / (nodeAnim->rotationTimestamps[rotationIndex] - nodeAnim->rotationTimestamps[rotationIndex - 1]);
					rotation = DirectX::XMQuaternionSlerp(nodeAnim->rotations[rotationIndex - 1], nodeAnim->rotations[rotationIndex], (float)t);
				}
			}

			// Scale
			{
				int scaleIndex = 0;
				for (int i = 0; i < nodeAnim->scaleTimestamps.size(); i++)
				{
					if (nodeAnim->scaleTimestamps[i] > _animationTick)
					{
						scaleIndex = i;
						break;
					}
				}

				if (scaleIndex == 0)
				{
					scale = nodeAnim->scales[0];
				}
				else
				{
					double t = (_animationTick - nodeAnim->scaleTimestamps[scaleIndex - 1]) / (nodeAnim->scaleTimestamps[scaleIndex] - nodeAnim->scaleTimestamps[scaleIndex - 1]);
					scale = DirectX::XMVectorLerp(nodeAnim->scales[scaleIndex - 1], nodeAnim->scales[scaleIndex], (float)t);
				}
			}

			node->transformMatrix = DirectX::XMMatrixAffineTransformation(scale, { 0,0,0,0 }, rotation, position);

			node->transform->SetLocalPosition(position);
			node->transform->SetLocalRotation(rotation);
			node->transform->SetLocalScale(scale);
		}

	}

	void DynamicModelRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		if (!_isActive)
		{
			return;
		}

		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_material->GetVertexShader()->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_material->GetPixelShader()->GetPixelShader(), nullptr, 0);

		// TODO : sampler 경고때문에 잠시주석처리. Sampler에 대해 다시 알아보자.
		deviceContext->PSSetSamplers(0, 1, _material->GetPixelShader()->GetAddressOfSampleState());

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_material->GetVertexShader()->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 상수 버퍼 세팅
		{
			// 버텍스 쉐이더
			unsigned int bufferNumber = 0;
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

			// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
			// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
			// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.

			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(_worldTM);
			DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(&det, _worldTM);

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_worldTM);
			DirectX::XMMATRIX wi = DirectX::XMMatrixTranspose(worldInverse);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->worldInverse = wi;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);


			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));

			// 카메라 버퍼 세팅
			{
				Camera* mainCam = Camera::GetMainCamera();
				// 버텍스 쉐이더
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				HR(deviceContext->Map(mainCam->GetCameraBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

				CameraBufferType* cameraBufferDataPtr = (CameraBufferType*)mappedResource.pData;

				cameraBufferDataPtr->cameraPosition = mainCam->GetPosition();
				cameraBufferDataPtr->padding = 0.0f;

				deviceContext->Unmap(mainCam->GetCameraBuffer(), 0);

				unsigned int bufferNumber = 1;

				deviceContext->VSSetConstantBuffers(bufferNumber, 1, mainCam->GetAddressOfCameraBuffer());
			}

			///
			bufferNumber = 2;
			HR(deviceContext->Map(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			NodeBufferType* nodeBufferDataPtr = (NodeBufferType*)mappedResource.pData;

			//SetNodeBuffer(_model->rootNode, nodeBufferDataPtr);
			SetNodeBuffer(_animatedRootNode, nodeBufferDataPtr);

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);


			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));


			bufferNumber = 3;
			HR(deviceContext->Map(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			BoneBufferType* boneBufferDataPtr = (BoneBufferType*)mappedResource.pData;

			// TODO : 이거 사실 둘 다 같은 본 데이터인건데 왜 매트릭스가 영행렬이 들어가있지..?
			//SetBoneBuffer(_model->rootNode, boneBufferDataPtr);
			SetBoneBuffer(_animatedRootNode, boneBufferDataPtr);
			testCount = 0;

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);


			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));

			/// 픽셀 쉐이더
			// PBR Data를 넘겨준다.
			bufferNumber = 0;

			HR(deviceContext->Map(_material->GetPixelShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			PBRBufferType* pbrBufferData = (PBRBufferType*)mappedResource.pData;

			pbrBufferData->metallic = _material->GetMetallic();
			pbrBufferData->roughness = _material->GetRoughness();
			pbrBufferData->useNormalMap = false;
			pbrBufferData->useMetallicMap = false;
			pbrBufferData->useRoughnessMap = false;
			pbrBufferData->useAOMap = false;

			if (_material->GetNormalTexture())
			{
				pbrBufferData->useNormalMap = true;
			}

			if (_material->GetMetallicTexture())
			{
				pbrBufferData->useMetallicMap = true;
			}

			if (_material->GetRoughnessTexture())
			{
				pbrBufferData->useRoughnessMap = true;
			}

			if (_material->GetAOTexture())
			{
				pbrBufferData->useAOMap = true;
			}

			deviceContext->Unmap(_material->GetPixelShader()->GetConstantBuffer(bufferNumber), 0);

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _material->GetPixelShader()->GetAddressOfConstantBuffer(bufferNumber));
		}

		// 렌더스테이트
		deviceContext->RSSetState(_material->GetRenderState());


		/// 그린다
		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		// 		if(_meshes == nullptr)
		// 		{
		// 			return;
		// 		}

		stride = sizeof(VertexSkinned);

		for (auto& mesh : _model->meshes)
		{
			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			if (_material->GetBaseColorTexture())
			{
				deviceContext->PSSetShaderResources(0, 1, _material->GetBaseColorTexture()->GetAddressOfTextureView());
			}
			if (_material->GetNormalTexture())
			{
				deviceContext->PSSetShaderResources(1, 1, _material->GetNormalTexture()->GetAddressOfTextureView());
			}
			if (_material->GetMetallicTexture())
			{
				deviceContext->PSSetShaderResources(2, 1, _material->GetMetallicTexture()->GetAddressOfTextureView());
			}
			if (_material->GetRoughnessTexture())
			{
				deviceContext->PSSetShaderResources(3, 1, _material->GetRoughnessTexture()->GetAddressOfTextureView());
			}
			if (_material->GetAOTexture())
			{
				deviceContext->PSSetShaderResources(4, 1, _material->GetAOTexture()->GetAddressOfTextureView());
			}

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}

		ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
		deviceContext->PSSetShaderResources(0, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(1, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(2, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(3, 1, nullSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(4, 1, nullSRV.GetAddressOf());
	}

	void DynamicModelRenderer::SetVertexShader(VertexShader* shader)
	{
		assert(_material);
		_material->SetVertexShader(shader);
	}

	void DynamicModelRenderer::SetPixelShader(PixelShader* shader)
	{
		assert(_material);
		_material->SetPixelShader(shader);
	}

	void DynamicModelRenderer::SetRenderState(ID3D11RasterizerState* renderState)
	{
		assert(_material);
		_material->SetRenderState(renderState);
	}

	void DynamicModelRenderer::CalcNodeWorldMatrix(Node* node)
	{
		node->CalcWorldMatrix();
		for (int i = 0; i < node->children.size(); i++)
		{
			CalcNodeWorldMatrix(node->children[i]);
		}
	}

	void DynamicModelRenderer::SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer)
	{
		// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
		// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
		// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.
		if (node->index >= 0)
		{
			// nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());
			nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->transform->GetWorldTM());
		}

		//nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->worldTM);
		for (int i = 0; i < node->children.size(); i++)
		{
			SetNodeBuffer(node->children[i], nodeBuffer);
		}
	}

	void DynamicModelRenderer::SetBoneBuffer(Node* node, BoneBufferType* boneBuffer)
	{
		Bone* bone = node->bindedBone;
		if (bone)
		{
			testCount++;
			boneBuffer->transformMatrix[bone->index] = DirectX::XMMatrixTranspose(bone->offsetMatrix);
		}

		for (int i = 0; i < node->children.size(); i++)
		{
			SetBoneBuffer(node->children[i], boneBuffer);
		}
	}

	Node* DynamicModelRenderer::CopyNodeData(Node* originalRootNode)
	{
		Node* rootNode = new Node();

		CopyNodeDataRecur(originalRootNode, rootNode);

		return rootNode;
	}

	void DynamicModelRenderer::CopyNodeDataRecur(Node* from, Node* to)
	{
		to->name = from->name;
		to->index = from->index;
		to->bindedBone = from->bindedBone;
		to->transformMatrix = from->transformMatrix;

		for (int i = 0; i < from->children.size(); i++)
		{
			Node* newNode = new Node();
			to->children.push_back(newNode);
			newNode->parent = to;
			CopyNodeDataRecur(from->children[i], newNode);
		}

		_animatedNodeMap.insert({ to->name,to });
	}

	void DynamicModelRenderer::BindTransformRecur(RocketTransform* transform, Node* node)
	{
		// 앞 단에서 게임오브젝트 생성하면서 이미 Transform을 세팅해놨다.
// 		DirectX::XMVECTOR outScale;
// 		DirectX::XMVECTOR outRotation;
// 		DirectX::XMVECTOR outTranslation;
// 
// 		DirectX::XMMatrixDecompose(&outScale, &outRotation, &outTranslation, node->transformMatrix);
// 
// 		transform->SetLocalScale(outScale);
// 		transform->SetLocalRotation(outRotation);
// 		transform->SetLocalPosition(outTranslation);

		node->transform = transform;

		for (int i = 0; i < node->children.size(); i++)
		{
			BindTransformRecur(transform->GetChild(i), node->children[i]);
		}
	}

	void DynamicModelRenderer::DeleteCopiedNodeRecur(Node* node)
	{
		for (int i = 0; i < node->children.size(); i++)
		{
			DeleteCopiedNodeRecur(node->children[i]);
		}

		delete node;
	}

	// 	DirectX::BoundingBox DynamicModelRenderer::GetBoundingBox() const
	// 	{
	// 		// WorldTM을 곱한 다음에 내보낸다.
	// 		DirectX::BoundingBox transformedBox;
	// 		_boundingBox.Transform(transformedBox, _armatureRootNode->transform->GetWorldTM());
	// 		transformedBox.Transform(transformedBox, 2.0f, { 0.0f,0.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f });
	// 		return transformedBox;
	// 	}

	DirectX::BoundingOrientedBox DynamicModelRenderer::GetBoundingBox() const
	{
		// WorldTM을 곱한 다음에 내보낸다.
		DirectX::BoundingOrientedBox transformedBox;
		_boundingBox.Transform(transformedBox, _armatureRootNode->transform->GetWorldTM());
		return transformedBox;
	}

	void DynamicModelRenderer::FindArmatureRootRecur(Node** out, Node* node) const
	{
		if (node->bindedBone)
		{
			*out = node;
			return;
		}

		for (auto& child : node->children)
		{
			FindArmatureRootRecur(out, child);
		}
	}

	void DynamicModelRenderer::LoadNormalTexture(std::string fileName)
	{
		_material->SetNormalTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::LoadMetallicTexture(std::string fileName)
	{
		_material->SetMetallicTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::LoadRoughnessTexture(std::string fileName)
	{
		_material->SetRoughnessTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::LoadAOTexture(std::string fileName)
	{
		_material->SetAmbientOcclusionTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::SetMetallic(float value)
	{
		_material->SetMetallic(value);
	}

	void DynamicModelRenderer::SetRoughness(float value)
	{
		_material->SetRoughness(value);
	}

}
