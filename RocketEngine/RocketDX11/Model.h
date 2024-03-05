#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
#include <vector>

#include "IResource.h"

namespace Rocket::Core
{
	class Mesh;

	class Model : public IResource
	{
	public:
		Model();
		~Model();

		std::string GetName() const { return name; }
		void SetName(std::string val) { name = val; }

		std::vector<Mesh*> GetMeshes() const { return meshes; }
		void SetMeshes(std::vector<Mesh*> val) { meshes = val; }

	private:
		std::string name;
		std::vector<Mesh*> meshes;
//		std::vector<Node*> nodes;	// 0 is the root node
		// TODO : Node를 Process 할 때 index를 그 때 매겨주자. 어때?
	};
}

