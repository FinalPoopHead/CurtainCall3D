#pragma once
#include <string>

namespace flt
{
	class Image
	{
	public:
		Image();
		Image(const std::wstring& path);

		void Set(const std::wstring& path);
		void GetSize(int& width, int& height) const;

	private:
		void Load();

		void LoadBmp();
		void LoadPng();
		void LoadJpg();

	private:
		std::wstring _path;
		std::wstring _name;
		int _width;
		int _height;
	};
}


