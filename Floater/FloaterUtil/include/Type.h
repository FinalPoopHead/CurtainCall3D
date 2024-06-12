#pragma once
#include <string_view>

#define FLT_WSTRING_IMPL(x) L##x
#define FLT_WSTRING(x) FLT_WSTRING_IMPL(x)

namespace flt 
{
	template<typename T>
	constexpr std::wstring_view TypeName() noexcept
	{
		constexpr std::wstring_view full_name{ FLT_WSTRING(__FUNCSIG__) };
		constexpr std::wstring_view prefix{ L"flt::TypeName<" };
		constexpr std::wstring_view suffix{ L">(void) noexcept" };

		constexpr auto left_marker_index = full_name.find(prefix);
		static_assert(left_marker_index != std::wstring_view::npos);
		constexpr auto start_index = left_marker_index + prefix.size();
		constexpr auto end_index = full_name.find(suffix, left_marker_index);
		static_assert(end_index != std::wstring_view::npos);
		constexpr auto length = end_index - start_index;

		return full_name.substr(start_index, length);
	}

}
