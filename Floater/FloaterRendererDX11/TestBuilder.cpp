#include "TestBuilder.h"



void flt::test::BuilderTest::Release()
{
	delete pTestData;
}

flt::test::BuilderTest* flt::test::BuilderTestBuilder::build() const
{
	BuilderTest* pBuilderTest = new BuilderTest();
	pBuilderTest->pTestData = new TestData(&t->_testData);
	return pBuilderTest;
}
