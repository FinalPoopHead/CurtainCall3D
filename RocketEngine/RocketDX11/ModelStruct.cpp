#include "ModelStruct.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"

namespace Rocket::Core
{
	std::vector<Mesh*> Model::GetMeshes()
	{
		return {};
	}

	std::vector<Mesh*> StaticModel::GetMeshes()
	{
		std::vector<Mesh*> meshes;
		for (auto& mesh : this->meshes)
		{
			meshes.emplace_back(mesh);
		}
		return meshes;
	}

	std::vector<Mesh*> DynamicModel::GetMeshes()
	{
		std::vector<Mesh*> meshes;
		for (auto& mesh : this->meshes)
		{
			meshes.emplace_back(mesh);
		}
		return meshes;
	}

}
