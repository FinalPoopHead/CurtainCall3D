#include "./include/internal/Component.h"
#include "./include/internal/GameObject.h"
#include "./include/internal/Scene.h"


void flt::ComponentBase::Enable()
{
	//_gameObject->_scene->AddEnableComponent(this, true);
	if (_isEnable)
	{
		return;
	}

	_isEnable = true;

	if (_gameObject->IsEnable())
	{
		OnEnable();
	}
}

void flt::ComponentBase::Disable()
{
	//_gameObject->_scene->AddEnableComponent(this, false);
	if (!_isEnable)
	{
		return;
	}

	if (_gameObject->IsEnable())
	{
		OnDisable();
	}
	_isEnable = false;
}

uint32 flt::ComponentBase::s_indexCounter = 0;
