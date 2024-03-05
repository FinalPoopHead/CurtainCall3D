#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "ObjectSystem.h"

namespace Rocket
{
	// 생성자 순서 조심할 것.
	// transform이 먼저 초기화 되지 않으면 Object 초기화할 때 문제 생김.
	GameObject::GameObject(std::string objName)
		:transform(*(new Transform())),
		objName(objName),
		_isActive(true),
		_isStarted(false),
		_components()
	{
		_components[typeid(transform).name()].push_back(&transform);
	}

	void GameObject::Destroy()
	{
		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				delete component;
			}
		}
	}

	void GameObject::Start()
	{
		if (!_isActive)
		{
			return;
		}

		if (_isStarted)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->Start();
				dynamic_cast<Component*>(component)->_isStarted = true;
			}
		}

		_isStarted = true;
	}

	void GameObject::FixedUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->FixedUpdate();
			}
		}
	}

	void GameObject::Update()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->Update();
			}
		}
	}

	void GameObject::LateUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->LateUpdate();
			}
		}
	}

	void GameObject::Finalize()
	{
		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->Finalize();
			}
		}
	}

	void GameObject::OnCollisionEnter()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnCollisionEnter();
			}
		}
	}

	void GameObject::OnCollisionStay()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnCollisionStay();
			}
		}
	}

	void GameObject::OnCollisionExit()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnCollisionExit();
			}
		}
	}

	void GameObject::OnDestroy()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnDestroy();
			}
		}
	}

	void GameObject::Enable()
	{
		Rocket::Core::ObjectSystem::Instance().AddEnable(this);
	}

	void GameObject::Disable()
	{
		Rocket::Core::ObjectSystem::Instance().AddDisable(this);
	}

	bool GameObject::IsActive() const
	{
		return _isActive;
	}

	void GameObject::FlushEnable()
	{
		_isActive = true;
		for (auto& child : transform.GetChildrenVec())
		{
			child->gameObject->FlushEnable();
		}
	}

	void GameObject::FlushDisable()
	{
		/*if (this == nullptr)
			return;*/

		_isActive = false;
		for (auto& child : transform.GetChildrenVec())
		{
			child->gameObject->FlushDisable();
		}
	}

	void GameObject::SetRestart()
	{
		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				dynamic_cast<Component*>(component)->SetRestart();
			}
		}

		_isStarted = false;
	}

	std::unordered_map<std::string, std::vector<Component*>>& GameObject::GetAllComponents()
	{
		return _components;
	}

}
