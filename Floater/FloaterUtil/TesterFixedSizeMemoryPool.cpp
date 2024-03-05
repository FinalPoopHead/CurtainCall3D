#include "TesterFixedSizeMemoryPool.h"

bool flt::test::TesterFixedSizeMemoryPool::Test()
{
	if (_pool._capacity != 64)
	{
		return false;
	}

	// 기본 할당 갯수인 64개 이상을 넣어보자.
	for (int i = 0; i < 65; ++i)
	{
		int* p = (int*)_pool.Alloc();
		*p = i;
	}

	if (_pool._capacity != 128)
	{
		return false;
	}

	_pool.FreeAll();

	// 할당 프리 연속적으로 해도 되는지 테스트.
	for (int i = 0; i < 200; ++i)
	{
		int* p = (int*)_pool.Alloc();
		_pool.Free(p);
	}

	if (_pool._capacity != 128)
	{
		return false;
	}

	return true;
}
