#include "ManagerObject.h"
#include "GameManagerController.h"

ManagerObject::ManagerObject()
{
	AddComponent<GameManagerController>(true);
}

ManagerObject::~ManagerObject()
{

}
