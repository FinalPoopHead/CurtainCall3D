#pragma once
#include "OwningPtr.h"

namespace flt
{
	template<typename T>
	class NonOwningPtr
	{
	public:
		constexpr NonOwningPtr() noexcept : NonOwningPtr(nullptr) { }
		constexpr NonOwningPtr(T* ptr) noexcept : _ptr(ptr) { }
		constexpr NonOwningPtr(const NonOwningPtr& other) noexcept : _ptr(other._ptr) { }
		constexpr NonOwningPtr(OwningPtr<T>& other) noexcept : _ptr(other.get()) { }
		constexpr NonOwningPtr(const OwningPtr<T>& other) noexcept : _ptr(other.get()) { }

		NonOwningPtr(NonOwningPtr&& other) = delete;

		constexpr NonOwningPtr& operator=(const NonOwningPtr& other) noexcept { _ptr = other._ptr; return *this; }
		constexpr NonOwningPtr& operator=(const OwningPtr<T>& other) noexcept { _ptr = other.get(); return *this; }
		~NonOwningPtr() noexcept {}

		constexpr [[nodiscard]] T* get() const noexcept { return _ptr; }
		constexpr [[nodiscard]] T* operator->() const { return _ptr; }
		constexpr [[nodiscard]] T& operator*() const { return *_ptr; }

	private:
		T* _ptr;
	};
}
