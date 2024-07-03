#include "HierarchyTest.h"
#include "../FloaterGameEngine/include/Input.h"
#include <iostream>


HierarchyTest::HierarchyTest()
{

}

void HierarchyTest::Update(float deltaSeconds)
{
	if (flt::GetKeyDown(flt::KeyCode::spacebar))
	{
		std::wcout << name << L" | ";
	}
}
