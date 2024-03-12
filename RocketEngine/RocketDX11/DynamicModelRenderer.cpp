#include "DynamicModelRenderer.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"
#include "ResourceManager.h"
#include "SkinnedMesh.h"

namespace Rocket::Core
{
	DynamicModelRenderer::DynamicModelRenderer()
		: _model(nullptr),
		_material(nullptr),
		_isActive(true),
		_worldTM(Matrix::Identity)
	{

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
		_animatedRootNode = CopyNodeData(_model->rootNode);
	}

	void DynamicModelRenderer::LoadTexture(std::string fileName)
	{
		_material->SetTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::UpdateAnimation(float deltaTime)
	{
		if (_model->animations.empty())
		{
			return;
		}

		// TODO : 외부에서 애니메이션 세팅하고 재생할 수 있게 바꿔야함. 지금은 임시로 첫번째 애니메이션만 실행함.
		Animation* anim = _model->animations.begin()->second;
		_nowAnimationName = anim->name;

		if (_animationTime == anim->duration)
		{
			return;
		}

		_animationTime += deltaTime;
		_animationTick = _animationTime * anim->ticksPerSecond;

		if (_animationTick > anim->duration)
		{
			if (_isLoop)
			{
				_animationTime -= anim->duration / anim->ticksPerSecond;
			}
			else
			{
				_animationTime = anim->duration / anim->ticksPerSecond;
			}
		}


		for (auto& nodeAnim : anim->nodeAnimations)
		{
			Vector3 position;
			Vector4 rotation;
			Vector3 scale;

			Node* node = _animatedNodeMap[nodeAnim->nodeName];

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
					float t = (_animationTick - nodeAnim->positionTimestamps[positionIndex - 1]) / (nodeAnim->positionTimestamps[positionIndex] - nodeAnim->positionTimestamps[positionIndex - 1]);
					position = DirectX::XMVectorLerp(nodeAnim->positions[positionIndex - 1], nodeAnim->positions[positionIndex], t);
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
					float t = (_animationTick - nodeAnim->rotationTimestamps[rotationIndex - 1]) / (nodeAnim->rotationTimestamps[rotationIndex] - nodeAnim->rotationTimestamps[rotationIndex - 1]);
					rotation = DirectX::XMQuaternionSlerp(nodeAnim->rotations[rotationIndex - 1], nodeAnim->rotations[rotationIndex], t);
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
					float t = (_animationTick - nodeAnim->scaleTimestamps[scaleIndex - 1]) / (nodeAnim->scaleTimestamps[scaleIndex] - nodeAnim->scaleTimestamps[scaleIndex - 1]);
					scale = DirectX::XMVectorLerp(nodeAnim->scales[scaleIndex - 1], nodeAnim->scales[scaleIndex], t);
				}
			}

			node->transformMatrix = DirectX::XMMatrixAffineTransformation(scale, { 0,0,0,0 }, rotation, position);
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

		deviceContext->PSSetSamplers(0, 1, _material->GetVertexShader()->GetAddressOfSampleState());

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

			SetBoneBuffer(_model->rootNode, boneBufferDataPtr);

			deviceContext->Unmap(_material->GetVertexShader()->GetConstantBuffer(bufferNumber), 0);


			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfConstantBuffer(bufferNumber));
			///
			// 픽셀 쉐이더
			bufferNumber = 0;

			HR(deviceContext->Map(_material->GetPixelShader()->GetConstantBuffer(bufferNumber), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			LightBufferType* lightBufferDataPtr = (LightBufferType*)mappedResource.pData;

			lightBufferDataPtr->ambientColor = { 0.15f,0.15f,0.15f,1.0f };
			lightBufferDataPtr->diffuseColor = { 1.0f,1.0f,1.0f,1.0f };
			lightBufferDataPtr->lightDirection = { 1.0f,0.0f,0.0f };
			lightBufferDataPtr->specularPower = 2.0f;
			lightBufferDataPtr->specularColor = { 1.0f,1.0f,1.0f,1.0f };

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

			deviceContext->PSSetShaderResources(0, 1, _material->GetTexture()->GetAddressOfTextureView());

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
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

	void DynamicModelRenderer::SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer)
	{
		// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
		// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
		// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.
		nodeBuffer->transformMatrix[node->index] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());
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



}
