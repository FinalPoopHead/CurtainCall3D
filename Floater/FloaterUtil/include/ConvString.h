#pragma once
#include <string>



namespace flt
{
	enum class strEncoding
	{
		ANSI,
		UTF8
	};

#ifdef UTF8
	//constexpr strEncoding defaultEncoding = strEncoding::UTF8;
#define FLT_DEFAULT_ENCODING (strEncoding::UTF8)
#else 
	//constexpr strEncoding defaultEncoding = strEncoding::ANSI;
#define FLT_DEFAULT_ENCODING (strEncoding::ANSI)
#endif // UTF8

	//std::wstring ToWstring(const std::string& str);
	//std::string ToString(const std::wstring& str);

	std::wstring ToWstring(const std::string& str, strEncoding encoding = FLT_DEFAULT_ENCODING);
	std::string ToString(const std::wstring& wstr, strEncoding encoding = FLT_DEFAULT_ENCODING);
}
