#pragma once
#include <vector>
#include "FloaterType.h"

namespace flt
{
	namespace test
	{
		class TesterFixedSizeMemoryPool;
	}

	class FixedSizeMemoryPool final
	{
	public:
		FixedSizeMemoryPool(uint32 singlePageSize);
		FixedSizeMemoryPool(uint32 singlePageSize, uint32 capacity);
		~FixedSizeMemoryPool();

		void* Alloc();
		void Free(void* ptr);
		void FreeAll();

	private:
		std::vector<byte*> _memoryChunks;
		int _contiguousStartIndex;

		const int _singlePageSize;
		int _capacity;
		int _allocatedCount;

		std::vector<void*> _freeStack;

		friend class test::TesterFixedSizeMemoryPool;
	};
}
