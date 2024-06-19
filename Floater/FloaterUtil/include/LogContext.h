#pragma once
#include <string>
#include <sstream>
#include <source_location>


namespace flt
{
	class LogContext
	{
	public:
		std::wstring GetLog() const&;
		std::wstring GetLog()&&;
		int GetGroupId() const noexcept;
		int GetSeverityId() const noexcept;
		int GetLine() const noexcept;
		int GetColumn() const noexcept;
		const char* GetFileName() const noexcept;
		const char* GetFunctionName() const noexcept;

	private:
		int _groupId;
		int _serverityId;
		std::source_location _location;
		std::wostringstream _stream;
	};
}
