#include "LogFormatObject.h"
#include <ctime>

namespace
{
	std::wstring getHour()
	{
		//auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		//auto time = std::localtime(&now);
		//return std::format(L"{:02}", time->tm_hour);

		time_t now = std::time(nullptr);
		tm buf{};
		localtime_s(&buf, &now);
		return std::format(L"{:02}", buf.tm_hour);
	}

	std::wstring getMinute()
	{
		time_t now = std::time(nullptr);
		tm buf{};
		localtime_s(&buf, &now);
		return std::format(L"{:02}", buf.tm_min);
	}

	std::wstring getSecond()
	{
		time_t now = std::time(nullptr);
		tm buf{};
		localtime_s(&buf, &now);
		return std::format(L"{:02}", buf.tm_sec);
	}

	std::wstring getDay()
	{
		time_t now = std::time(nullptr);
		tm buf{};
		localtime_s(&buf, &now);
		return std::format(L"{:02}", buf.tm_mday);
	}

	std::wstring getMonth()
	{
		time_t now = std::time(nullptr);
		tm buf{};
		localtime_s(&buf, &now);
		return std::format(L"{:02}", buf.tm_mon + 1);
	}

	std::wstring getYear()
	{
		time_t now = std::time(nullptr);
		tm buf{};
		localtime_s(&buf, &now);
		return std::format(L"{:04}", buf.tm_year + 1900);
	}
}

namespace flt
{
	FormatObject Hour{ getHour };
	FormatObject Minute{ getMinute };
	FormatObject Second{ getSecond };
	FormatObject Day{ getDay };
	FormatObject Month{ getMonth };
	FormatObject Year{ getYear };
	//FormatObject FileName;
	//FormatObject FunctionName;
	//FormatObject LineNumber;
	//FormatObject LogLevel;
}
