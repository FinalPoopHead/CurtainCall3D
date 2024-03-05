#pragma once
#include "../../FloaterMath/include/Vector2f.h"
#include "../../FloaterMath/include/Vector3f.h"
#include "../../FloaterMath/include/Vector4f.h"
#include "Transform.h"
#include "RawMaterial.h"
#include "Resource.h"
#include "IBuilder.h"
#include <vector>
#include <string>


namespace flt
{
	struct RawVertex
	{
		RawVertex() = default;
		RawVertex(const Vector3f& pos, const Vector2f uvs[10], const Vector3f& normal)
			: pos(pos), uvs(), normal(normal), binormal(), tangent(), boneIndice(), boneWeights()
		{
			for (int i = 0; i < 10; ++i)
			{
				this->uvs[i] = uvs[i];
			}
		}

		std::partial_ordering operator<=>(const RawVertex& other) const
		{
			std::partial_ordering result = pos <=> other.pos;
			if (result == std::partial_ordering::less || result == std::partial_ordering::greater)
			{
				return result;
			}

			for (int i = 0; i < 10; ++i)
			{
				result = uvs[i] <=> other.uvs[i];
				if (result == std::partial_ordering::less || result == std::partial_ordering::greater)
				{
					return result;
				}
			}

			return normal <=> other.normal;
			return result;
		}
		bool operator==(const RawVertex& other) const
		{
			if (*this <=> other == std::partial_ordering::equivalent)
			{
				return true;
			}
			return false;
		}

		Vector3f pos;
		Vector2f uvs[10];
		Vector3f normal;
		Vector3f binormal;
		Vector3f tangent;
		std::vector<unsigned int> boneIndice;
		std::vector<float> boneWeights;
	};

	struct RawMesh
	{
		RawMesh() : vertices(), indices(), material() {}
		void Release() {}

		std::vector<RawVertex> vertices;
		std::vector<int> indices;
		RawMaterial material;
	};

	template struct Resource<RawMesh>;

	struct RawMeshBuilder : public IBuilder<RawMesh>
	{
		RawMeshBuilder(const std::wstring& filePath, const std::wstring& name) :
			IBuilder<RawMesh>(filePath + name),
			vertices(),
			indices() {}

		virtual RawMesh* build() const override;

		std::vector<RawVertex> vertices;
		std::vector<int> indices;
	};
}
