#pragma once
#include <format>
#include <source_location>

#include <iostream>


namespace flt
{
	enum class LogLevel
	{
		trace,
		debug,
		info,
		warn,
		err,
		critical,
		off,
		MAX
	};

	//class Logger
	//{
	//public:
	//	template<typename... Args>
	//	void Log(LogLevel msgLevel, std::source_location location, std::wstring_view fmt, Args... args)
	//	{

	//	}
	//};

	template<typename... Args>
	void Log(LogLevel msgLevel, std::source_location location, std::wstring_view fmt, Args... args)
	{
		std::cout 
			<< "fileName : " 
			<< location.file_name() 
			<< " line : " << location.line()
			<< " func : " << location.function_name() 
			<< "\n";
		std::wcout << std::vformat(fmt, std::make_wformat_args(std::forward<Args>(args)...)) << "\n";
	}



	template<typename... Args>
	struct info
	{
		info(std::wstring_view fmt, Args&&... args, std::source_location location = std::source_location::current())
		{
			Log(LogLevel::info, location, fmt, std::forward<Args>(args)...);
		}
	};

	template <typename... Args>
	info(std::wstring_view, Args&&...) -> info<Args...>;
}
