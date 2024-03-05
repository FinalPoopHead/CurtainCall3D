#pragma once
#include <iostream>
#include "../FloaterRendererCommon/include/IBuilder.h"
#include "../FloaterRendererCommon/include/Resource.h"
#include "../FloaterRendererCommon/include/ResourceMgr.h"

#include "../FloaterUtil/include/ITester.h"


namespace flt
{
	namespace test
	{
		class BuilderTester;


		struct TestData
		{
			TestData(int* pI) : testData(pI)
			{
				std::cout << "TestData Created" << std::endl;
				*testData += 1;
			}

			~TestData()
			{
				std::cout << "TestData Destroyed" << std::endl;
				*testData -= 1;
			}

			int* testData;
		};

		struct BuilderTest
		{
			void Release();

			TestData* pTestData;
		};

		template struct Resource<BuilderTest>;

		struct BuilderTestBuilder : public IBuilder<BuilderTest>
		{
			BuilderTestBuilder(const std::wstring filePath) : IBuilder<BuilderTest>(filePath), t(nullptr) {}

			virtual BuilderTest* build() const override;

		public:
			BuilderTester* t;
		};



		class BuilderTester : public ITester
		{
		public:
			virtual bool Test() override
			{
				using namespace flt::test;

				// 동일한 빌더로 여러개 만들기.
				BuilderTestBuilder builder1(L"test");
				builder1.t = this;


				Resource<BuilderTest> test1(builder1);
				Resource<BuilderTest> test2(builder1);
				Resource<BuilderTest> test3(builder1);
				if (_testData != 1)
				{
					ASSERT(false, L"빌더로 여러개 만들었으나 1번만 생성되지 않음.");
					return false;
				}

				// 하나 해제 후 move시에 ref카운트 0이 되는지 확인.
				// Release()가 된 test1은 nullptr이 되어 이를 move를 하게 되면 나머지도 해제가 되어야함.
				test1.Release();
				test2 = std::move(test1);
				test3 = std::move(test2);
				if (_testData != 0)
				{
					ASSERT(false, L"ref카운트가 0이되고 해제가 되어야하나 해제되지 않음.");
					return false;
				}

				return true;
			}


		public:
			ResourceMgr _resourceMgr;

			int _testData = 0;
		};
	}
}
