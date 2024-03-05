#pragma once
#include <string>
#include "RawScene.h"

namespace flt
{
	class FBXLoader;
	class GLTFLoader;
	class AssimpLoader;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();

		bool Load(std::wstring path, RawScene* outRawScene);

	private:
		FBXLoader* _pFBXLoader;
		GLTFLoader* _pGLTFLoader;
		AssimpLoader* _pAssimpLoader;
	};
}
