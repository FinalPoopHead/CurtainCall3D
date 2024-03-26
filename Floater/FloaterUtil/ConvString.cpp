#define _CRT_SECURE_NO_WARNINGS
#include "./include/convString.h"
#include <vector>
#include <locale>
#include <windows.h>


std::wstring flt::ToWstring(const std::string& str, strEncoding encoding)
{
	std::wstring ret;
	switch (encoding)
	{
		case strEncoding::ANSI:
		{
			int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), 0, 0);
			ret.resize(size);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &ret[0], size);
		}
		break;
		case strEncoding::UTF8:
		{
			int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), 0, 0);
			ret.resize(size);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &ret[0], size);
		}
		break;
		default:
			break;
	}

	return ret;
}

std::string flt::ToString(const std::wstring& wstr, strEncoding encoding)
{
	std::string ret;
	switch (encoding)
	{
		case strEncoding::ANSI:
		{
			int size = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), 0, 0, 0, 0);
			ret.resize(size);
			WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &ret[0], size, 0, 0);
		}
		break;
		case strEncoding::UTF8:
		{
			int size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), 0, 0, 0, 0);
			ret.resize(size);
			WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &ret[0], size, 0, 0);
		}
		break;
		default:
			break;
	}

	return ret;
}
