#pragma once

namespace flt
{
	template<typename T>
	struct DefaultDestructor
	{
		constexpr void operator()(T* ptr) const noexcept
		{
			delete ptr;
		}
	};

	template<typename T>
	struct DefaultDestructor<T[]>
	{
		constexpr void operator()(T* ptr) const noexcept
		{
			delete[] ptr;
		}
	};

	template<typename T, typename Destructor = DefaultDestructor<T>>
	class OwningPtr
	{
	public:
		constexpr OwningPtr() noexcept : _ptr(nullptr) {}
		constexpr OwningPtr(T* ptr) noexcept : _ptr(ptr) {}
		constexpr OwningPtr(OwningPtr&& other) noexcept : _ptr(other._ptr) { other._ptr = nullptr; }
		~OwningPtr() { _destructor(_ptr); }
		constexpr OwningPtr& operator=(OwningPtr&& other) noexcept
		{
			if (this != &other)
			{
				_destructor(_ptr);
				_ptr = other._ptr;
				other._ptr = nullptr;
			}
			return *this;
		}
		constexpr OwningPtr& operator=(T* ptr) noexcept
		{
			_destructor(_ptr);
			_ptr = ptr;
			return *this;
		}
		OwningPtr(const OwningPtr&) = delete;
		OwningPtr& operator=(const OwningPtr&) = delete;

		[[nodiscard]] constexpr T* get() const noexcept { return _ptr; }
		[[nodiscard]] constexpr T* operator->() const { return _ptr; }
		[[nodiscard]] constexpr T& operator*() const { return *_ptr; }

	private:
		T* _ptr;
		Destructor _destructor;
	};
}


