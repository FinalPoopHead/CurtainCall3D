#pragma once
#include "FloaterType.h"
#include <type_traits>


namespace flt
{
	template<typename T, uint64 N>
	class StaticArray
	{
	public:
		StaticArray() : _data() {}

		template<typename... Args>
		StaticArray(Args... args) requires (sizeof...(Args) <= N) && (std::same_as<T, Args> && ...)
			: _data{ args... }
		{

		}

		[[nodiscard]] constexpr T& operator[](uint64 index) noexcept
		{
			return _data[index];
		}

		[[nodiscard]] constexpr const T& operator[](uint64 index) const noexcept
		{
			return _data[index];
		}

		[[nodiscard]] constexpr uint64 Size() const noexcept
		{
			return N;
		}

		[[nodiscard]] T* Data() noexcept
		{
			return _data;
		}

		[[nodiscard]] constexpr const T* Data() const noexcept
		{
			return _data;
		}

		[[nodiscard]] T& At(uint64 index) noexcept
		{
			ASSERT(index < N, "Index out of range");

			return _data[index];
		}

		[[nodiscard]] const T& At(uint64 index) const noexcept
		{
			ASSERT(index < N, "Index out of range");

			return _data[index];
		}

	private:
		T _data[N];
	};

	template<typename T>
	class StaticArray<T, 0>
	{
	public:
		static_assert(false, "Array size must be greater than 0");
	};

	template<typename T, typename... U>
	StaticArray(T, U...) -> StaticArray<T, sizeof...(U) + 1>;
}
