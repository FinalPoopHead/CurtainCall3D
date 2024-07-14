#pragma once
#include "FloaterMacro.h"
#include "FloaterType.h"
#include <vector>
#include <type_traits>


namespace flt
{
	template<typename T>
	class SparseSet
	{
#pragma region Nasted
	public:
		class iterator
		{
			friend class SparseSet<T>;
		public:
			constexpr iterator() noexcept : iterator(nullptr, -1, 0) {}
			constexpr iterator(SparseSet<T>* set, size_t denseIndex, unsigned int version) noexcept
				: _set(set), _denseIndex(denseIndex), _version(version) {}
			constexpr iterator(const iterator& other) noexcept
				: _set(other._set), _denseIndex(other._denseIndex), _version(other._version) {}
			constexpr iterator(iterator&& other) noexcept
				: _set(other._set), _denseIndex(other._denseIndex), _version(other._version)
			{
				other._set = nullptr;
				other._denseIndex = 0;
				other._version = 0;
			}

		public:
			constexpr iterator& operator=(const iterator& other) noexcept
			{
				_set = other._set;
				_denseIndex = other._denseIndex;
				_version = other._version;
				return *this;
			}
			constexpr iterator& operator=(iterator&& other) noexcept
			{
				_set = other._set;
				_denseIndex = other._denseIndex;
				_version = other._version;
				other._set = nullptr;
				other._denseIndex = 0;
				other._version = 0;
				return *this;
			}

			constexpr iterator& operator++() noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				++_denseIndex;
				return *this;
			}
			constexpr iterator operator++(int) noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				iterator temp = *this;
				++_denseIndex;
				return temp;
			}

			constexpr bool operator==(const iterator& other) const noexcept
			{
				return _set == other._set && _denseIndex == other._denseIndex && _version == other._version;
			}
			constexpr bool operator!=(const iterator& other) const noexcept
			{
				return _set != other._set || _denseIndex != other._denseIndex || _version != other._version;
			}

			[[nodiscard]] constexpr T& operator*() noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return _set->_dense[_denseIndex].value;
			}
			[[nodiscard]] constexpr const T& operator*() const noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return _set->_dense[_denseIndex].value;
			}
			[[nodiscard]] constexpr T* operator->() noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return &_set->_dense[_denseIndex].value;
			}
			[[nodiscard]] constexpr const T* operator->() const noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return &_set->_dense[_denseIndex].value;
			}

			[[nodiscard]] constexpr int GetIndex() const noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return _set->_dense[_denseIndex].sparseIndex;
			}

		private:
			SparseSet<T>* _set;
			size_t _denseIndex;
			unsigned int _version;
		};

		class const_iterator
		{
			friend class SparseSet<T>;
		public:
			constexpr const_iterator() noexcept : const_iterator(nullptr, -1, 0) {}
			constexpr const_iterator(const SparseSet<T>* set, size_t denseIndex, unsigned int version) noexcept
				: _set(set), _denseIndex(denseIndex), _version(version) {}
			constexpr const_iterator(const const_iterator& other) noexcept
				: _set(other._set), _denseIndex(other._denseIndex), _version(other._version) {}
			constexpr const_iterator(const_iterator&& other) noexcept
				: _set(other._set), _denseIndex(other._denseIndex), _version(other._version)
			{
				other._set = nullptr;
				other._denseIndex = 0;
				other._version = 0;
			}

		public:
			constexpr const_iterator& operator=(const const_iterator& other) noexcept
			{
				_set = other._set;
				_denseIndex = other._denseIndex;
				_version = other._version;
				return *this;
			}
			constexpr const_iterator& operator=(const_iterator&& other) noexcept
			{
				_set = other._set;
				_denseIndex = other._denseIndex;
				_version = other._version;
				other._set = nullptr;
				other._denseIndex = 0;
				other._version = 0;
				return *this;
			}

			constexpr const_iterator& operator++() noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				++_denseIndex;
				return *this;
			}
			constexpr const_iterator operator++(int) noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				const_iterator temp = *this;
				++_denseIndex;
				return temp;
			}

			constexpr bool operator==(const const_iterator& other) const noexcept
			{
				return _set == other._set && _denseIndex == other._denseIndex && _version == other._version;
			}
			constexpr bool operator!=(const const_iterator& other) const noexcept
			{
				return _set != other._set || _denseIndex != other._denseIndex || _version != other._version;
			}

			[[nodiscard]] constexpr const T& operator*() noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return _set->_dense[_denseIndex].value;
			}
			[[nodiscard]] constexpr const T& operator*() const noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return _set->_dense[_denseIndex].value;
			}
			[[nodiscard]] constexpr const T* operator->() noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return &_set->_dense[_denseIndex].value;
			}
			[[nodiscard]] constexpr const T* operator->() const noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return &_set->_dense[_denseIndex].value;
			}

			[[nodiscard]] constexpr int GetIndex() const noexcept
			{
				ASSERT(_set != nullptr, "invaild iterator");
				ASSERT(_version == _set->_version, "invaild iterator");
				return _set->_dense[_denseIndex].sparseIndex;
			}

		private:
			const SparseSet<T>* _set;
			size_t _denseIndex;
			unsigned int _version;
		};

		//class reverse_iterator
		//{
		//	friend class SparseSet<T>;
		//};

		//class const_reverse_iterator
		//{
		//	friend class SparseSet<T>;
		//};

	private:
		struct DenseData
		{
		public:
			constexpr DenseData() noexcept : DenseData(T{}, -1) {}
			constexpr DenseData(const T& value, int sparseIndex) noexcept : value(value), sparseIndex(sparseIndex) {}
			constexpr DenseData(T&& value, int sparseIndex) noexcept : value(value), sparseIndex(sparseIndex) {}

			template<typename... Args>
			constexpr DenseData(int sparseIndex, Args&&... args) noexcept : value(std::forward<Args>(args)...), sparseIndex(sparseIndex) {}


		public:
			T value;
			int sparseIndex;
		};
#pragma endregion

		friend class SparseSet<T>::iterator;
		friend class SparseSet<T>::const_iterator;
		//friend class SparseSet<T>::reverse_iterator;
		//friend class SparseSet<T>::const_reverse_iterator;

	public:
		SparseSet();
		SparseSet(unsigned int capacity);
		~SparseSet() {}

		void Reserve(uint32 size) noexcept;

		//iterator Insert(T&& value) noexcept;
		iterator Insert(uint32 sparseIndex, T&& value) noexcept;

		std::vector<iterator> Find(const T& value) noexcept;

		//void Remove(const T& value) noexcept;
		//void Remove(const iterator& it) noexcept;
		void Erase(uint32 index) noexcept;
		void Erase(const iterator& it) noexcept;
		void Clear() noexcept;
		iterator PushBack(T&& value) noexcept;
		template<typename... Args>
		iterator EmplaceBack(Args&&... args) noexcept;
		void PopBack() noexcept;

		//[[nodiscard]] const T& operator[](uint32 sparseIndex) const noexcept { return _dense[_sparse[sparseIndex]].value; }
		[[nodiscard]] T& operator[](uint32 sparseIndex) noexcept;

		[[nodiscard]] const T& At(uint32 sparseIndex) const noexcept { return _dense[_sparse[sparseIndex]].value; }
		[[nodiscard]] T& At(uint32 sparseIndex) noexcept { return _dense[_sparse[sparseIndex]].value; }

		[[nodiscard]] const T& Front() const noexcept { return _dense.front().value; }
		[[nodiscard]] T& Front() noexcept { return _dense.front().value; }
		[[nodiscard]] T& Back() noexcept { return _dense.back().value; }
		[[nodiscard]] const T& Back() const noexcept { return _dense.back().value; }

		[[nodiscard]] bool Empty() const noexcept { return _dense.size() == 0; }
		[[nodiscard]] uint32 Size() const noexcept { return _dense.size(); }
		[[nodiscard]] uint32 Capacity() const noexcept { return _capacity; }

		[[nodiscard]] const_iterator begin() const noexcept;
		[[nodiscard]] iterator begin() noexcept;
		[[nodiscard]] const_iterator end() const noexcept;
		[[nodiscard]] iterator end() noexcept;

	private:
		std::vector<DenseData> _dense;
		std::vector<int> _sparse;
		std::vector<size_t> _free;
		uint32 _version;
		uint32 _capacity;
	};

	template<typename T>
	flt::SparseSet<T>::SparseSet() : SparseSet(1)
	{
	}

	template<typename T>
	flt::SparseSet<T>::SparseSet(unsigned int capacity)
		: _dense()
		, _sparse()
		, _free()
		, _version(0)
		, _capacity(0)
	{
		if (capacity < 1)
		{
			capacity = 1;
		}
		Reserve(capacity);
	}

	template<typename T>
	void flt::SparseSet<T>::Reserve(uint32 capacity) noexcept
	{
		if (capacity == _capacity)
		{
			return;
		}
		if (capacity < _capacity)
		{
			return;
		}

		_dense.reserve(capacity);
		_sparse.resize(capacity, -1);
		_free.clear();
		_free.reserve(capacity);

		uint32 i = capacity - 1;
		while (i >= _capacity)
		{
			if (_sparse[i] == -1)
			{
				_free.push_back(i);
			}

			if(i == _capacity)
			{
				break;
			}
			--i;
		}
		_capacity = capacity;
	}

	template<typename T>
	flt::SparseSet<T>::iterator flt::SparseSet<T>::Insert(uint32 sparseIndex, T&& value) noexcept
	{
		++_version;
		while(sparseIndex >= _capacity)
		{
			Reserve(_capacity * 2);
		}

		_free.erase(std::remove(_free.begin(), _free.end(), sparseIndex), _free.end());
		_sparse[sparseIndex] = (int)_dense.size();
		_dense.emplace_back(std::forward<T>(value), sparseIndex);

		return iterator(this, _dense.size() - 1, _version);
	}

	template<typename T>
	std::vector<typename flt::SparseSet<T>::iterator> flt::SparseSet<T>::Find(const T& value) noexcept
	{
		std::vector<iterator> result;
		for (size_t i = 0; i < _dense.size(); ++i)
		{
			if (_dense[i].value == value)
			{
				ASSERT(_dense[i].sparseIndex != -1, "invaild datastructure");
				result.emplace_back(this, i, _version);
			}
		}

		return result;
	}

	//template<typename T>
	//void flt::SparseSet<T>::Remove(const iterator& it) noexcept
	//{
	//	ASSERT(it._set == this, "invaild iterator");
	//	ASSERT(it._version == _version, "invaild iterator");

	//	++_version;
	//	int sparseIndex = _dense[it._denseIndex].sparseIndex;
	//	_sparse[sparseIndex] = -1;
	//	_dense[it._denseIndex] = _dense.back();
	//	_dense.pop_back();
	//	_sparse[_dense[it._denseIndex].sparseIndex] = it._denseIndex;
	//	_free.push_back(sparseIndex);
	//}

	template<typename T>
	void flt::SparseSet<T>::Erase(uint32 sparseIndex) noexcept
	{
		if (sparseIndex < 0 || sparseIndex > _capacity)
		{
			ASSERT(false, "invaild index");
			return;
		}

		++_version;
		uint32 denseIndex = _sparse[sparseIndex];
		_sparse[sparseIndex] = -1;

		if (denseIndex < _dense.size() - 1)
		{
			_dense[denseIndex] = _dense.back();
			_sparse[_dense[denseIndex].sparseIndex] = denseIndex;
		}
		_dense.pop_back();
		_free.push_back(sparseIndex);
	}


	template<typename T>
	void flt::SparseSet<T>::Erase(const iterator& it) noexcept
	{
		uint32 sparseIndex = _dense[it._denseIndex].sparseIndex;
		Erase(sparseIndex);
	}


	template<typename T>
	void flt::SparseSet<T>::Clear() noexcept
	{
		_dense.clear();
		_free.clear();
		_version = 0;

		for (auto& i : _sparse)
		{
			i = -1;
		}

		for(int i = _capacity - 1; i >= 0; --i)
		{
			_free.push_back(i);
		}
	}

	template<typename T>
	flt::SparseSet<T>::iterator flt::SparseSet<T>::PushBack(T&& value) noexcept
	{
		_version++;
		if (_free.empty())
		{
			Reserve(_capacity * 2);
		}

		int sparseIndex = _free.back();
		_free.pop_back();

		int denseIndex = (int)_dense.size();
		_sparse[sparseIndex] = denseIndex;
		_dense.emplace_back(std::forward<T>(value), sparseIndex);

		return iterator(this, denseIndex, _version);
	}

	template<typename T>
	template<typename... Args>
	flt::SparseSet<T>::iterator flt::SparseSet<T>::EmplaceBack(Args&&... args) noexcept
	{
		++_version;
		if (_free.empty())
		{
			Reserve(_capacity * 2);
		}

		int sparseIndex = (int)_free.back();
		_free.pop_back();

		int denseIndex = (int)_dense.size();
		_sparse[sparseIndex] = denseIndex;
		_dense.emplace_back(sparseIndex, std::forward<Args>(args)...);

		return iterator(this, denseIndex, _version);
	}

	template<typename T>
	void flt::SparseSet<T>::PopBack() noexcept
	{
		++_version;

		int sparseIndex = _dense.back().sparseIndex;
		_sparse[sparseIndex] = -1;

		_dense.pop_back();
		_free.push_back(sparseIndex);
	}


	template<typename T>
	T& flt::SparseSet<T>::operator[](uint32 sparseIndex) noexcept
	{
		if(_sparse[sparseIndex] == -1)
		{
			Insert(sparseIndex, T{});
			//ASSERT(false, "invaild index");
			//return _dense[0].value;
		}
		return _dense[_sparse[sparseIndex]].value;
	}


	template<typename T>
	flt::SparseSet<T>::const_iterator flt::SparseSet<T>::begin() const noexcept
	{
		const_iterator iter{ this, 0, _version };
		return iter;
	}

	template<typename T>
	flt::SparseSet<T>::iterator flt::SparseSet<T>::begin() noexcept
	{
		iterator iter{ this, 0, _version };
		return iter;
	}

	template<typename T>
	flt::SparseSet<T>::const_iterator flt::SparseSet<T>::end() const noexcept
	{
		const_iterator iter{ this, _dense.size(), _version };
		return iter;
	}

	template<typename T>
	flt::SparseSet<T>::iterator flt::SparseSet<T>::end() noexcept
	{
		iterator iter{ this, _dense.size(), _version };
		return iter;
	}
}
