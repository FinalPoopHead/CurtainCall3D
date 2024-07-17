#include "./include/FLTween.h"
#include <iostream>


flt::IFLTween::IFLTween()
{
	std::cout << "Create Tween\n";
}

flt::IFLTween::~IFLTween()
{
	std::cout << "Delete Tween\n";
}

