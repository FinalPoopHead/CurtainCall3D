#include "AdvantageCube.h"
#include "CubeController.h"

AdvantageCube::AdvantageCube()
{
	AddComponent<CubeController>(true);
}

AdvantageCube::~AdvantageCube()
{

}
