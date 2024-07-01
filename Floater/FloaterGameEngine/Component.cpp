#include "./include/internal/Component.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"


void flt::ComponentBase::Enable()
{
	_gameObject->_scene->AddEnableComponent(this, true);
}

void flt::ComponentBase::Disable()
{
	_gameObject->_scene->AddEnableComponent(this, false);
}

bool flt::ComponentBase::IsEnable() const
{
	return _isEnable && _gameObject->isEnable();
}

int flt::ComponentBase::s_indexCounter = 0;
