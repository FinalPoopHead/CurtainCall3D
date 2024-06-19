#pragma once
#include <string>
#include <format>
#include "Log.h"




namespace flt
{
	class PatternFormatter
	{
	public:
		void format();
		void SetPattern(const std::wstring& pattern);
	};
}

template<>
struct std::formatter<flt::PatternFormatter> : std::formatter<std::wstring>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		auto iter = ctx.begin();
		auto end = ctx.end();
		while(iter != end && *iter != L'}')
		{
			switch (*iter)
			{
				case L'H' :
				{

				}
				break;
				case L'M':
				{

				}
				break;
				case L'S':
				{

				}
				break;
				default:
				{

				}
			}
			++iter;
		}
		return ctx.begin();
	}

	template<typename FormatContext>
	auto format(const flt::PatternFormatter& p, FormatContext& ctx)
	{
		return format_to(ctx.out(), L"PatternFormatter");
	}

	std::wstring_view object;
};
