#pragma once
#include "Mesh.h"

namespace Rocket::Core
{
	class SkinnedMesh : public Mesh
	{
	public:
		SkinnedMesh();
		SkinnedMesh(std::vector<VertexSkinned> vertices, std::vector<UINT> indices);

		virtual void CreateBuffers() override;

	protected:
		std::vector<VertexSkinned> _vertices;
	};
}
