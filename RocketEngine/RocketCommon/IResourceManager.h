#pragma once
#include <vector>
#include <string>

namespace Rocket::Core
{
	struct RawModel;

	class IResourceManager
	{
	public:
		virtual ~IResourceManager() {};

		virtual void LoadModel(const std::string& fileName, const RawModel* rawModel) = 0;
	};
}
