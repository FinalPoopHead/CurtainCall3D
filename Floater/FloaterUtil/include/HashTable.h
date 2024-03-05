#pragma once
#include <functional>

namespace flt
{
	namespace test
	{
		class TesterHashTable;
	}

	template<typename Key, typename Value>
	class HashTable
	{
#pragma region Nasted
	private:
		struct Node
		{
			int64 version;
			Key key;
			Value value;
		};
		class iterator
		{

		};
#pragma endregion
	public:
		HashTable();
		~HashTable();

	private:
		void Rehash();
		float LoadFactor() const;

		uint32 bucketIndex(uint64 hashed) const
		{
			uint64 mask111 = (1u << (_numbits + 1u)) - 1u;
			uint64 mask011 = (1u << _numbits) - 1u;

			uint32 les = (hashed & mask111) < _numbuckets;

			return ((les * (hashed & mask111)) + ((1u - les) * (hashed >> (_numbits + 1u)) & mask011));
		}

	private:
		std::function<int(const Key&)> _hash;
		int _size;
		Node* _table;
		int64 _version;

		uint32* _buckets;
		int _numbuckets;
		uint32 _numbits;



		friend class test::TesterHashTable;
	};

	template<typename Key, typename Value>
	void flt::HashTable<Key, Value>::Rehash()
	{
		_numbuckets *= 2;

		// log2 계산
		uint64 pow2 = 1;
		_numbits = -1;
		while (pow2 <= _numbuckets)
		{
			pow2 = pow2 << 1;
			_numbits++;
		

		uint32* newBuckets = new uint32[_numbuckets];
		Node* newTable = new Node[_numbuckets];

		// 데이터 복사, 재할당 등.

		delete[] _buckets;
		delete[] _table;

		_buckets = newBuckets;
		_table = newTable;
	}

	template<typename Key, typename Value>
	float flt::HashTable<Key, Value>::LoadFactor() const
	{
		return (float)_size / (_capacity - 1);
	}

}

template<typename Key, typename Value>
flt::HashTable<Key, Value>::HashTable() : 
	_numbuckets(512), _size(0),
	_table(new Node[_numbuckets]), _buckets(new uint32[_numbuckets]),
	_version(0)
{

}

template<typename Key, typename Value>
flt::HashTable<Key, Value>::~HashTable()
{
	delete[] _table;
	delete[] _buckets;
}
