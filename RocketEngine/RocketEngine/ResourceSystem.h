#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "Singleton.h"

namespace Rocket::Core
{
	class IResourceManager;
	class FBXLoaderToRaw;
	class RawModel;
}

namespace Rocket::Core
{
	class ResourceSystem: public Singleton<ResourceSystem>
	{
		friend Singleton;
	private:
		ResourceSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		~ResourceSystem();

		void Initialize();

		RawModel* GetModel(const std::string& fileName);

		void Clear();

	private:
		IResourceManager* _resourceManager;
		FBXLoaderToRaw& _fbxLoader;
		std::unordered_map<std::string, RawModel*> _modelMap;
	};
}
