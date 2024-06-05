#include "DarkCube.h"
#include "CubeController.h"

DarkCube::DarkCube()
{
	AddComponent<CubeController>(true);
}

DarkCube::~DarkCube()
{

}
