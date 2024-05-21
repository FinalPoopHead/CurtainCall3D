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

std::wstring flt::ToWstring(int value)
{
	wchar_t buffer[256];
	swprintf(buffer, 256, L"%d", value);
	std::wstring ret = buffer;
	return ret;
}

std::wstring flt::ToWstring(float value)
{
	wchar_t buffer[256];
	swprintf(buffer, 256, L"%f", value);
	std::wstring ret = buffer;
	return ret;
}

std::wstring flt::ToWstring(double value)
{
	wchar_t buffer[256];
	swprintf(buffer, 256, L"%lf", value);
	std::wstring ret = buffer;
	return ret;
}

std::string flt::ToString(int value)
{
	char buffer[256];
	sprintf(buffer, "%d", value);
	std::string ret = buffer;
	return ret;
}


std::string flt::ToString(float value)
{
	char buffer[256];
	sprintf(buffer, "%f", value);
	std::string ret = buffer;
	return ret;
}

std::string flt::ToString(double value)
{
	char buffer[256];
	sprintf(buffer, "%lf", value);
	std::string ret = buffer;
	return ret;
}
