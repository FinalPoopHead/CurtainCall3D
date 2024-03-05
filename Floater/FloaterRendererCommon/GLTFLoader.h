#pragma once
#include <string>

namespace flt
{
	struct RawScene;

	class GLTFLoader
	{
	public:
		void Load(const std::wstring& filePath, RawScene* outRawScene);

	private:
		void LoadGLTF(const std::wstring& filePath);
		void LoadGLB(const std::wstring& filePath);
	};
}


