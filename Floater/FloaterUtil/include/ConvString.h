#pragma once
#include <string>

namespace flt
{
	std::wstring ConvertToWstring(const std::string& str);
	std::string ConvertToString(const std::wstring& str);
}
