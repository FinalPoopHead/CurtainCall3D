#pragma once
#include "IMaterial.h"
#include <vector>

namespace Rocket::Core
{
	class IResourceManager
	{
	public:
		virtual ~IResourceManager() {};
		virtual void LoadFile(const char* filePath) const = 0;
		virtual void UnloadResource() const = 0;

		virtual IMaterial* CreateMaterial(MaterialDesc& materialDesc) const = 0;

		virtual std::vector<std::string> GetMeshKeys() const = 0;
	};
}