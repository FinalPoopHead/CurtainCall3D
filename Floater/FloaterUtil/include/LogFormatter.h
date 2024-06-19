#pragma once
#include <string>


namespace flt
{
	class LogFormatter
	{
	public:
		std::wstring Format(std::wstring_view logMsg);
	};

	std::wstring FormatHelper(std::wstring_view fmt, const LogContext& ctx);
}
