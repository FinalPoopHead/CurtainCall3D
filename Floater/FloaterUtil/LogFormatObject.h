#pragma once
#include <functional>
#include <string>
#include <format>



namespace flt
{
	class FormatObject
	{
	public:
		FormatObject(std::function<std::wstring()> function) : _function(function) {}

		std::wstring operator()() const
		{
			return _function();
		}
	private:
		std::function<std::wstring()> _function;
	};

	extern FormatObject Hour;
	extern FormatObject Minute;
	extern FormatObject Second;
	extern FormatObject Day;
	extern FormatObject Month;
	extern FormatObject Year;
	//extern FormatObject FileName;
	//extern FormatObject FunctionName;
	//extern FormatObject LineNumber;
	//extern FormatObject LogLevel;
}

template<>
struct std::formatter<flt::FormatObject>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		return ctx.begin();
	}

	template<typename FormatContext>
	auto format(const flt::FormatObject& p, FormatContext& ctx)
	{
		return std::format_to(ctx.out(), L"{}", p());
	}
};
