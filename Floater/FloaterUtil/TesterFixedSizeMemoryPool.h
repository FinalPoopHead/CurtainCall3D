#pragma once
#include "./include/FixedSizeMemoryPool.h"
#include "./include/ITester.h"

namespace flt
{
	namespace test
	{

		class TesterFixedSizeMemoryPool : public ITester
		{
		public:
			TesterFixedSizeMemoryPool() : _pool(sizeof(int)) {}
			~TesterFixedSizeMemoryPool() {}

			virtual bool Test() override;

		private:
			FixedSizeMemoryPool _pool;
		};

	}
}
