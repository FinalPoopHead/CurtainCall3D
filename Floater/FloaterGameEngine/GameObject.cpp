#include "./include/GameObject.h"
#include "./include/Scene.h"

bool flt::GameObject::AddComponent(Component* component)
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

void flt::GameObject::RemoveComponent(Component* component)
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
