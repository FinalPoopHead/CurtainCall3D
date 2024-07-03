#include "HierarachyMgr.h"
#include "../FloaterGameEngine/include/Input.h"
#include <iostream>


HierarachyMgr::HierarachyMgr()
	: _hierarchyTestArr()
{
	for (int i = 0; i < 10; ++i)
	{
		//_hierarchyTestArr[i] = _scene->CreateGameObject<HierarchyTest>(true);
		_hierarchyTestArr[i] = flt::CreateGameObject<HierarchyTest>(true);
		_hierarchyTestArr[i]->name = std::to_wstring(i);
	}

	_hierarchyTestArr[0]->SetParent(_hierarchyTestArr[1]);
	_hierarchyTestArr[1]->SetParent(_hierarchyTestArr[2]);

	_hierarchyTestArr[3]->SetParent(_hierarchyTestArr[6]);
	_hierarchyTestArr[4]->SetParent(_hierarchyTestArr[6]);
	_hierarchyTestArr[5]->SetParent(_hierarchyTestArr[6]);
}

void HierarachyMgr::PreUpdate(float deltaSeconds)
{
	if (GetKeyDown(flt::KeyCode::key1))
	{
		std::cout << "\nkey1  ";

		if (_hierarchyTestArr[1]->IsEnable())
		{
			_hierarchyTestArr[1]->Disable();
		}
		else
		{
			_hierarchyTestArr[1]->Enable();
		}

	}

	if (GetKeyDown(flt::KeyCode::key2))
	{
		std::cout << "\nkey2  ";

		if (_hierarchyTestArr[2]->IsEnable())
		{
			_hierarchyTestArr[2]->Disable();
		}
		else
		{
			_hierarchyTestArr[2]->Enable();
		}
	}

	if(GetKeyDown(flt::KeyCode::key3))
	{
		std::cout << "\nkey3  ";

		if (_hierarchyTestArr[6]->IsEnable())
		{
			_hierarchyTestArr[6]->Disable();
		}
		else
		{
			_hierarchyTestArr[6]->Enable();
		}
	}

	if (GetKeyDown(flt::KeyCode::key4))
	{
		std::cout << "\nkey4  ";

		_hierarchyTestArr[2]->Destroy();
	}
	if (flt::GetKeyDown(flt::KeyCode::spacebar))
	{
		std::cout << "\n";
	}
}

void HierarachyMgr::PostUpdate(float deltaSeconds)
{
	if (flt::GetKeyDown(flt::KeyCode::spacebar))
	{
		std::cout << "\n";
	}
}
