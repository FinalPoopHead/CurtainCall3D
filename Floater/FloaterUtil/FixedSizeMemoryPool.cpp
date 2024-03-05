#include "./include/FixedSizeMemoryPool.h"
#include <memory>

static constexpr int _chunkBitShift = 6;
static constexpr int _chunkSize = (1 << 6);

#define ALIGN_64(X) (((X) + 63) & ~63)


flt::FixedSizeMemoryPool::FixedSizeMemoryPool(uint32 singlePageSize) : FixedSizeMemoryPool(singlePageSize, 64)
{

}

flt::FixedSizeMemoryPool::FixedSizeMemoryPool(uint32 singlePageSize, uint32 capacity) :
	_memoryChunks(static_cast<size_t>(ALIGN_64(capacity))),
	_contiguousStartIndex(0),
	_singlePageSize(singlePageSize), 
	_capacity(ALIGN_64(capacity)),
	_allocatedCount(0),
	_freeStack()
{
	_freeStack.reserve(_capacity);

	for (auto& chunk : _memoryChunks)
	{
		chunk = (byte*)malloc((size_t)_singlePageSize * _chunkSize);
	}
}

flt::FixedSizeMemoryPool::~FixedSizeMemoryPool()
{
	for (auto& chunk : _memoryChunks)
	{
		if (chunk)
		{
			free(chunk);
		}
	}

	_memoryChunks.clear();
}

void* flt::FixedSizeMemoryPool::Alloc()
{
	++_allocatedCount;

	// 먼저 FreeStack을 확인 후 있음 Top을 return;
	if (_freeStack.size() > 0)
	{
		void* ptr = _freeStack.back();
		_freeStack.pop_back();
		return ptr;
	}

	if (_allocatedCount > _capacity)
	{
		_memoryChunks.emplace_back((byte*)malloc(_singlePageSize * _chunkSize));
		if (_memoryChunks.back() == nullptr)
		{
			_memoryChunks.pop_back();
			--_allocatedCount;
			return nullptr;
		}

		_capacity += _chunkSize;
	}

	return _memoryChunks[_allocatedCount >> _chunkBitShift] + (_allocatedCount & (_chunkSize - 1)) * _singlePageSize;

	++_contiguousStartIndex;
}

void flt::FixedSizeMemoryPool::Free(void* ptr)
{
	_freeStack.push_back(ptr);
}

void flt::FixedSizeMemoryPool::FreeAll()
{
	_allocatedCount = 0;
	_contiguousStartIndex = 0;
	_freeStack.clear();
}
