#pragma once
#include <type_traits>

namespace flt
{
	template <typename T, typename Alloc>
	concept Allocator_ = requires(Alloc a, T * p, size_t n, const T & value, T && rvalue) {
		{ a.Allocate(n) } -> std::same_as<T*>;
		{ a.DeAllocate(p) };
		{ a.Construct(p) };
		{ a.Construct(p, value) };
		{ a.Construct(p, std::move(rvalue)) };
		{ a.Destroy(p) };
	};

	template<typename T>
	class Allocator
	{
		T* Allocate(size_t elementNum)
		{
			return (T*)malloc(sizeof(T) * elementNum);
		}

		void DeAllocate(T* target)
		{
			free(target);
		}

		void Construct(T* target)
		{
			new ((void*)target) T();
		}

		void Construct(T* target, const T& value)
		{
			new ((void*)target) T(value);
		}

		void Construct(T* target, T&& value) noexcept
		{
			new ((void*)target) T(std::move(value));
		}

		void Destroy(T* target)
		{
			target->~T();
		}
	};
}
