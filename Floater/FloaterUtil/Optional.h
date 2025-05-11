#pragma once
#include <stdexcept>
#include <type_traits>

namespace flt
{
	template<typename T>
	class Optional
	{
	public:
		constexpr Optional() noexcept : _hasValue(false), _storage() {}

		constexpr Optional(const Optional& other)
			requires std::is_copy_constructible_v<T>
		: _hasValue(other._hasValue), _storage()
		{
			if (other._hasValue)
			{
				new (&_storage) T(other.Value());
			}
		}
		constexpr Optional(Optional&& other) noexcept
			requires std::is_move_constructible_v<T>
		: _hasValue(other._hasValue), _storage()
		{
			if (other._hasValue)
			{
				new (&_storage) T(std::move(other.Value()));
			}
		}

		template <typename... Args>
			requires (sizeof...(Args) > 1 || !(std::is_same_v<Optional, std::decay_t<Args>...>))
		explicit constexpr Optional(Args&&... args) noexcept : _hasValue(true), _storage()
		{
			new (&_storage) T(std::forward<Args>(args)...);
		}

		~Optional() { Reset(); }

		Optional& operator=(const Optional& other)
			requires std::is_copy_assignable_v<T>
		{
			if (this != &other)
			{
				Reset();
				_hasValue = other._hasValue;
				if (_hasValue)
				{
					new (&_storage) T(other.Value());
				}
			}
			return *this;
		}
		Optional& operator=(Optional&& other) noexcept
			requires std::is_move_assignable_v<T>
		{
			if (this != &other)
			{
				Reset();
				_hasValue = other._hasValue;
				if (_hasValue)
				{
					new (&_storage) T(std::move(other.Value()));
				}
			}
			return *this;
		}

		Optional& operator=(const T& value)
			requires std::is_copy_assignable_v<T>
		{
			Reset();
			new (&_storage) T(value);
			_hasValue = true;
			return *this;
		}
		Optional& operator=(T&& value) noexcept
			requires std::is_move_assignable_v<T>
		{
			Reset();
			new (&_storage) T(std::move(value));
			_hasValue = true;
			return *this;
		}

	public:
		bool HasValue() const noexcept { return _hasValue; }
		T& Value()
		{
			if (!_hasValue)
			{
				throw std::runtime_error("No value present");
			}

			return *std::launder(reinterpret_cast<T*>(&_storage));
		}
		const T& Value() const
		{
			if (!_hasValue)
			{
				throw std::runtime_error("No value present");
			}

			return *std::launder(reinterpret_cast<const T*>(&_storage));
		}
		void Reset()
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				if (_hasValue)
				{
					std::launder(reinterpret_cast<T*>(&_storage))->~T();
				}
			}

			_hasValue = false;
		}

		T& operator*() { return Value(); }
		const T& operator*() const { return Value(); }
		T* operator->() { return &Value(); }
		const T* operator->() const { return &Value(); }

		template<typename... Args>
		constexpr void Emplace(Args&&... args)
		{
			Reset();
			new (&_storage) T(std::forward<Args>(args)...);
			_hasValue = true;
		}

	private:
		alignas(T) unsigned char _storage[sizeof(T)];
		bool _hasValue;
	};
}
