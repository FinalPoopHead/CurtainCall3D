#pragma once
#include "FloaterMacro.h"

namespace flt
{
	namespace test
	{
		class ITester
		{
		public:
			virtual ~ITester() {}
			virtual bool Test() = 0;
		};
	}
}
