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

void HierarchyTest::OnEnable()
{
	std::wcout << name << L" en| ";
}

void HierarchyTest::OnDisable()
{
	std::wcout << name << L" dis| ";
}
