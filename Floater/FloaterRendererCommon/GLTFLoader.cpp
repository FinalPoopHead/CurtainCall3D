#include "GLTFLoader.h"
#include "../FloaterUtil/include/FloaterMacro.h"


void flt::GLTFLoader::Load(const std::wstring& filePath, RawScene* outRawScene)
{
	ASSERT(outRawScene, "outRawScene is nullptr");

	std::wstring extension = filePath.substr(filePath.find_last_of(L".") + 1);
	for (auto& c : extension)
	{
		c = towlower(c);
	}

	if (extension == L"gltf")
	{
		LoadGLTF(filePath);
	}
	else if (extension == L"glb")
	{
		LoadGLB(filePath);
	}
	else
	{
		ASSERT(false, "Not Supported File");
	}
}

void flt::GLTFLoader::LoadGLTF(const std::wstring& filePath)
{

}

void flt::GLTFLoader::LoadGLB(const std::wstring& filePath)
{

}
