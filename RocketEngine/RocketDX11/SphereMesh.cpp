#include <vector>
#include "SphereMesh.h"
#include "GraphicsMacro.h"
#include "VertexStruct.h"

namespace Rocket::Core
{

	SphereMesh::SphereMesh()
	{

	}

	SphereMesh::~SphereMesh()
	{
// 		_vertexBuffer.Reset();
// 		_indexBuffer.Reset();
	}

	void SphereMesh::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void SphereMesh::BuildGeometryBuffers(ID3D11Device* device, float radius, UINT sliceCount, UINT stackCount)
	{
		_vertexType = eVertexType::LIGHT_VERTEX;

		std::vector<LightVertex> vertices;
		std::vector<UINT> indices;

		float phiStep = DirectX::XM_PI / stackCount;
		float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

		// Create vertices
		for (UINT i = 0; i <= stackCount; ++i)
		{
			float phi = i * phiStep;

			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				LightVertex vertex;
				vertex.position.x = radius * sinf(phi) * cosf(theta);
				vertex.position.y = radius * cosf(phi);
				vertex.position.z = radius * sinf(phi) * sinf(theta);

				vertex.uv.x = theta / (2.0f * DirectX::XM_PI);
				vertex.uv.y = phi / DirectX::XM_PI;

				vertex.normal.x = sinf(phi) * cosf(theta);
				vertex.normal.y = cosf(phi);
				vertex.normal.z = sinf(phi) * sinf(theta);

				vertices.push_back(vertex);
			}
		}

		// Create indices
		for (UINT i = 0; i < stackCount; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				UINT baseIndex = i * (sliceCount + 1) + j;
				indices.push_back(baseIndex);
				indices.push_back(baseIndex + 1);
				indices.push_back(baseIndex + sliceCount + 1);

				indices.push_back(baseIndex + 1);
				indices.push_back(baseIndex + sliceCount + 2);
				indices.push_back(baseIndex + sliceCount + 1);
			}
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(LightVertex) * (UINT)vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices.data();
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * (UINT)indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices.data();
		HR(device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));

		_vertexCount = (int)vertices.size();
		_indexCount = (int)indices.size();

		_vertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"SPHERE v-Buffer") - 1, L"SPHERE v-Buffer");
		_indexBuffer->SetPrivateData(WKPDID_D3DDebugObjectNameW, sizeof(L"SPHERE i-Buffer") - 1, L"SPHERE i-Buffer");
	}

}
