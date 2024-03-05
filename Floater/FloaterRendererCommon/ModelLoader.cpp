#include "../FloaterUtil/include/FloaterMacro.h"
#include "./include/RawScene.h"
#include "./include/ModelLoader.h"
#include "FBXLoader.h"
#include "GLTFLoader.h"
#include "AssimpLoader.h"
#include <filesystem>

flt::ModelLoader::ModelLoader() :
	_pFBXLoader(new(std::nothrow) FBXLoader()),
	_pGLTFLoader(new(std::nothrow) GLTFLoader()),
	_pAssimpLoader(new(std::nothrow) AssimpLoader())
{

}

flt::ModelLoader::~ModelLoader()
{
	delete _pFBXLoader;
	delete _pGLTFLoader;
	delete _pAssimpLoader;
}

bool flt::ModelLoader::Load(std::wstring path, RawScene* outRawScene)
{
	if (!std::filesystem::exists(path))
	{
		ASSERT(false, "File Not Exist");
		return false;
	}

	size_t pos = path.find_last_of(L".");
	std::wstring extension = path.substr(pos + 1);
	for (auto& c : extension)
	{
		c = towlower(c);
	}

	_pAssimpLoader->Load(path, outRawScene);

	return true;

	if (extension == L"fbx")
	{
		_pFBXLoader->Load(path, outRawScene);
	}
	else if (extension == L"gltf" || extension == L"glb")
	{
		_pGLTFLoader->Load(path, outRawScene);
	}
	else
	{
		ASSERT(false, "Not Supported File");
		return false;
	}

	return true;
}
