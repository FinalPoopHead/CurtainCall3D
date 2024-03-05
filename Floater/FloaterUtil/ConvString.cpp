#define _CRT_SECURE_NO_WARNINGS
#include "./include/convString.h"
#include <atlstr.h>

#include <format>
#include <locale>	
#include <codecvt>


std::wstring flt::ConvertToWstring(const std::string& str)
{
	CA2W wcstr = CA2W(str.c_str());
	std::wstring ret = static_cast<std::wstring>(wcstr);
	return ret;
}

std::string flt::ConvertToString(const std::wstring& str)
{
	CW2A cstr = CW2A(str.c_str());
	std::string ret = static_cast<std::string>(cstr);
	return ret;
}
