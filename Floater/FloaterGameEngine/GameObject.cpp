#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"

flt::GameObject::GameObject()
	: _scene()
	, name()
	, _components()
	, _isEnable(true)
	, _index(-1)
	, _updateIndex(-1)
{

}

flt::GameObject::~GameObject()
{

}

flt::GameObject* flt::GameObject::GetParent() const
{
	GameObject* parent = static_cast<GameObject*>(tr.GetParent()->GetOwner());

	return parent;
}

void flt::GameObject::SetParent(GameObject* parent)
{
	if (!parent)
	{
		tr.SetParent(nullptr);
		return;
	}

	tr.SetParent(&parent->tr);
}

std::vector<flt::GameObject*> flt::GameObject::GetChildren() const
{
	std::vector<flt::GameObject*> ret;
	auto children = tr.GetChildren();

	for(auto& child : children)
	{
		ret.push_back(static_cast<GameObject*>(child->GetOwner()));
	}

	return ret;
}

flt::GameObject* flt::GameObject::GetChild(int index) const
{
	GameObject* child = static_cast<GameObject*>(tr.GetChild(index)->GetOwner());
	return child;
}

void flt::GameObject::AddChild(GameObject* child)
{
	ASSERT(child, "child is nullptr");
	tr.AddChild(&child->tr);
}

void flt::GameObject::RemoveChild(GameObject* child)
{
	ASSERT(child, "child is nullptr");
	child->tr.SetParent(nullptr);
}

void flt::GameObject::RemoveAllChildren()
{
	Transform* child = tr.GetChild(0);
	while (child)
	{
		tr.GetChild(0)->SetParent(nullptr);
		child = tr.GetChild(0);
	}
}

void flt::GameObject::RemoveChild(int index)
{
	tr.GetChild(index)->SetParent(nullptr);
}

void flt::GameObject::Enable()
{
	if (_isEnable)
	{
		return;
	}

	_isEnable = true;

	for (auto& component : _components)
	{
		if (component->_isEnable)
		{
			component->OnEnable();
		}
	}
	OnEnable();
	_scene->AddEnableGameObject(this, true);
}

void flt::GameObject::Disable()
{
	if (!_isEnable)
	{
		return;
	}

	_isEnable = false;

	for (auto& component : _components)
	{
		if (component->_isEnable)
		{
			component->OnDisable();
		}
	}
	OnDisable();
	_scene->AddEnableGameObject(this, false);
}

void flt::GameObject::Destroy()
{
	_scene->AddDestroyGameObject(this);
}

bool flt::GameObject::AddComponent(ComponentBase* component)
{
	int index = component->GetIndex();

	if (_components.Capacity() <= index)
	{
		_components.Reserve(index + 1);
	}

	if (_components[index] != nullptr)
	{
		return false;
	}

	_components[index] = component;
	component->_gameObject = this;
	return true;
}

void flt::GameObject::RemoveComponent(ComponentBase* component)
{
	int index = component->GetIndex();
	if (_components.Capacity() <= index)
	{
		return;
	}

	if (_components[index] == component)
	{
		_components[index] = nullptr;
		component->_gameObject = nullptr;
	}
}
