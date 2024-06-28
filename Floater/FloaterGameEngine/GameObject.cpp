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

void flt::GameObject::Enable()
{
	_scene->AddEnableGameObject(this, true);
}

void flt::GameObject::Disable()
{
	_scene->AddEnableGameObject(this, false);
}

void flt::GameObject::Destroy()
{
	_scene->AddDestroyGameObject(this);
}

bool flt::GameObject::AddComponent(ComponentBase* component)
{
	int index = component->GetIndex();
	
	if(_components.size() <= index)
	{
		_components.resize(index + 1);
	}
	
	if(_components[index] != nullptr)
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
	if(_components.size() <= index)
	{
		return;
	}

	if(_components[index] == component)
	{
		_components[index] = nullptr;
		component->_gameObject = nullptr;
	}
}
