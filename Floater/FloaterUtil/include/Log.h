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

	template<typename T>
	T& UnMove(T&& t)
	{
		return t;
	}

	//class Logger
	//{
	//public:
	//	template<typename... Args>
	//	void Log(LogLevel msgLevel, std::source_location location, std::wstring_view fmt, Args... args)
	//	{

	//	}
	//};

	template<typename... Args>
	void Log(LogLevel msgLevel, std::source_location location, std::wstring_view fmt, Args&&... args)
	{
		// TODO : 로그를 남길 level이 맞는지 체크
		// TODO : 로그를 남길 방법에 대한 구현 (콘솔, 파일)
		// TODO : 별도 쓰레드에서 IO작업 처리하도록 변경
		// TODO : 로그 포매터에 맞춰서 출력하는 기능 추가
		std::cout 
			<< "fileName : " 
			<< location.file_name() 
			<< " line : " << location.line()
			<< " func : " << location.function_name() 
			<< "\n";
		std::wcout << std::vformat(fmt, std::make_wformat_args(UnMove(args)...)) << "\n";
	}

	// Helper class for logging

	template<typename... Args>
	struct Info
	{
		Info(std::wstring_view fmt, Args&&... args, std::source_location location = std::source_location::current())
		{
			Log(LogLevel::info, location, fmt, std::forward<Args>(args)...);
		}
	};

	template <typename... Args>
	Info(std::wstring_view, Args&&...) -> Info<Args...>;

	/*template<typename... Args>
	struct Debug
	{
		Debug(std::wstring_view fmt, Args&&... args, std::source_location location = std::source_location::current())
		{
			Log(LogLevel::debug, location, fmt, std::forward<Args&&>(args)...);
		}
	};

	template <typename... Args>
	Debug(std::wstring_view, Args&&...) -> Debug<Args...>;

	template<typename... Args>
	struct Warn
	{
		Warn(std::wstring_view fmt, Args&&... args, std::source_location location = std::source_location::current())
		{
			Log(LogLevel::warn, location, fmt, std::forward<Args>(args)...);
		}
	};

	template <typename... Args>
	Warn(std::wstring_view, Args&&...) -> Warn<Args...>;

	template<typename... Args>
	struct Error
	{
		Error(std::wstring_view fmt, Args&&... args, std::source_location location = std::source_location::current())
		{
			Log(LogLevel::err, location, fmt, std::forward<Args>(args)...);
		}
	};

	template <typename... Args>
	Error(std::wstring_view, Args&&...) -> Error<Args...>;

	template<typename... Args>
	struct Critical
	{
		Critical(std::wstring_view fmt, Args&&... args, std::source_location location = std::source_location::current())
		{
			Log(LogLevel::critical, location, fmt, std::forward<Args>(args)...);
		}
	};

	template <typename... Args>
	Critical(std::wstring_view, Args&&...) -> Critical<Args...>;*/
}
