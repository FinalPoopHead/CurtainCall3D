#pragma once
#include "Mesh.h"

namespace Rocket::Core
{
	class StaticMesh : public Mesh
	{
	public:
		StaticMesh();
		StaticMesh(std::vector<Vertex> vertices, std::vector<UINT> indices);

		virtual void CreateBuffers() override;
		virtual void SetNode(Node* node) override;

	protected:
		std::vector<Vertex> _vertices;
	};
}

