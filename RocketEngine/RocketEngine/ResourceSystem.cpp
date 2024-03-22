#include "ResourceSystem.h"
#include "GraphicsSystem.h"
#include "../RocketCommon/IResourceManager.h"
#include "../RocketCommon/FBXLoaderToRaw.h"
#include "../RocketCommon/RawModelStruct.h"

namespace Rocket::Core
{
	ResourceSystem::ResourceSystem()
		:_fbxLoader(*(new FBXLoaderToRaw()))
	{

	}

	ResourceSystem::~ResourceSystem()
	{
		delete &_fbxLoader;
	}

	void ResourceSystem::Initialize()
	{
		_resourceManager = GraphicsSystem::Instance()._resourceManager;
	}

	RawModel* ResourceSystem::GetModel(const std::string& fileName)
	{
		if(_modelMap.find(fileName) == _modelMap.end())
		{
			RawModel* model = _fbxLoader.LoadFBXFile(fileName);
			_modelMap.insert({ fileName,model });
			_resourceManager->LoadModel(fileName, model);		// 그래픽스엔진의 리소스매니저에도 모델을 로드한다.
		}

		return _modelMap.at(fileName);
	}



}
